#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <regex.h>
#include <limits.h>
#include <inttypes.h>
#include <getopt.h>
#include <pthread.h>
#include <search.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "iowrapper.h"
#include "protocol.h"
#include "listener_socket.h"
#include "queue.h"
#include "rwlock.h"

#define BUFFER_SIZE     4096
#define PATH_MAX        4096
#define DEFAULT_THREADS 4
#define QUEUE_SIZE      64
#define MAX_URIS        2048

// method shortcuts for readability
#define PUT     0
#define GET     1
#define INVALID -1

// Helpers implemented under main
// print HTTP return headers
void httperror(int client_fd, int code);
void httpsuccess_header(int client_fd, int code, size_t content_length);
// get the rwlock for a given uri
static rwlock_t *get_uri_lock(const char *uri);
// handle connection logic and thread management
void handle_connection(int client_fd);
// handle PUT request
int handle_put_request(int client_fd, const char *uri, size_t content_length);
// handle GET request
int handle_get_request(int client_fd, const char *uri);
// worker thread function
void *worker_thread(void *arg);
// print to audit log
void add_to_log(const char *method, const char *uri, int status_code, const char *request_id);

static Listener_Socket_t *listen_socket = NULL; // listener socket
pthread_t *worker_threads = NULL; // collect worker thread Ids
queue_t *work_queue = NULL; // thread safe queue implementation

pthread_mutex_t audit_log_lock = PTHREAD_MUTEX_INITIALIZER; // mutex for audit log
static pthread_mutex_t uri_hash_lock = PTHREAD_MUTEX_INITIALIZER; // mutex for uri hash table

int thread_count = DEFAULT_THREADS; // default thread count

FILE *debug_log_fp = NULL;

static void on_shutdown(int sig) {
    (void) sig; // suppress unused variable warning
    // Clean up the listener socket
    if (listen_socket != NULL) {
        ls_delete(&listen_socket);
        listen_socket = NULL;
    }
    queue_delete(&work_queue); // delete the queue
    hdestroy(); // destroy the hash table
    for (int i = 0; i < thread_count; i++) {
        pthread_cancel(worker_threads[i]); // cancel all worker threads
    }
    free(worker_threads); // free the worker threads
    worker_threads = NULL; // set to NULL for safety
    exit(EXIT_SUCCESS);
}

void clean_up_writer_unlock(void *arg) {
    if (arg) {
        writer_unlock((rwlock_t *) arg);
    }
}

void clean_up_reader_unlock(void *arg) {
    if (arg) {
        reader_unlock((rwlock_t *) arg);
    }
}

static void clean_up_mutex_unlock(void *arg) {
    if (arg) {
        pthread_mutex_unlock((pthread_mutex_t *) arg);
    }
}

int main(int argc, char *argv[]) {

    int opt;
    char *endptr;

    // Parse arguments with getopt
    while ((opt = getopt(argc, argv, "t:")) != -1) {
        switch (opt) {
        case 't':
            thread_count = strtol(optarg, &endptr, 10);
            if (*endptr != '\0' || thread_count <= 0) {
                fprintf(debug_log_fp, "Invalid thread count\n");
                return EXIT_FAILURE;
            }
            break;
        default:
            fprintf(debug_log_fp, "Usage: %s [-t threads (4)] <port (1-65535)>\n", argv[0]);
            return EXIT_FAILURE;
        }
    }

    if (optind + 1 != argc) { // missing port argument
        fprintf(debug_log_fp, "Usage: %s [-t threads (4)] <port (1-65535)>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Inside main(), after parsing arguments but before starting threads/listening
    debug_log_fp = fopen("server_debug.log", "a"); // "a" for append mode
    if (debug_log_fp == NULL) {
        perror("Failed to open server_debug.log");
        // Decide if this is a fatal error or if you can continue without debug logging
    }

    // Check if the port is a valid number
    long port_long = strtol(argv[optind], &endptr, 10);
    if (*endptr != '\0' || port_long <= 0 || port_long > 65535) {
        fprintf(debug_log_fp, "Invalid Port\n");
        return EXIT_FAILURE;
    }
    uint16_t port = (uint16_t) port_long;

    if (!hcreate(MAX_URIS)) {
        fprintf(debug_log_fp, "Failed to create hash table\n");
        return EXIT_FAILURE;
    }

    // initialize queue and threads
    work_queue = queue_new(QUEUE_SIZE);
    if (work_queue == NULL) {
        fprintf(debug_log_fp, "Failed to create work queue\n");
        return EXIT_FAILURE;
    }

    worker_threads = malloc(sizeof(pthread_t) * thread_count);
    if (worker_threads == NULL) {
        fprintf(debug_log_fp, "Failed to allocate worker threads\n");
        queue_delete(&work_queue);
        return EXIT_FAILURE;
    }

    // create 4-user_in threads
    for (int i = 0; i < thread_count; i++) {
        if (pthread_create(&worker_threads[i], NULL, worker_thread, NULL) != 0) {
            fprintf(debug_log_fp, "Failed to create worker thread\n");
            free(worker_threads);
            queue_delete(&work_queue);
            return EXIT_FAILURE;
        }
    }

    // init socket
    signal(SIGINT, on_shutdown); // handle SIGINT to clean up
    signal(SIGTERM, on_shutdown); // handle SIGTERM to clean up
    signal(SIGPIPE, SIG_IGN); // ignore SIGPIPE signal

    // Create the socket
    listen_socket = ls_new(port);
    if (listen_socket == NULL) {
        fprintf(debug_log_fp, "Listening: Invalid Port\n");
        return EXIT_FAILURE;
    }

    // printf("Server listening on port %d\n", port); // DEBUG

    // begin listening
    while (1) {
        int client_fd = ls_accept(listen_socket);
        if (client_fd < 0) {
            fprintf(debug_log_fp, "Failed to accept connection\n");
            continue; // Skip to the next iteration
        }

        int *client_fd_ptr = malloc(sizeof(int));
        if (client_fd_ptr == NULL) {
            fprintf(debug_log_fp, "Failed to allocate memory for client_fd\n");
            close(client_fd);
            continue; // Skip to the next iteration
        }
        *client_fd_ptr = client_fd;
        // Push the client_fd onto the worker queue

        if (!queue_push(work_queue, client_fd_ptr)) {
            fprintf(debug_log_fp, "Failed to push client_fd to queue\n");
            free(client_fd_ptr);
            close(client_fd);
            continue; // Skip to the next iteration
        }
    }

    // Clean up and return
    ls_delete(&listen_socket);
    if (listen_socket != NULL) {
        fprintf(debug_log_fp, "Failed to delete listener socket\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

void httperror(int client_fd, int code) {
    const char *status_text;

    switch (code) {
    case 400: status_text = "Bad Request"; break;
    case 403: status_text = "Forbidden"; break;
    case 404: status_text = "Not Found"; break;
    case 500: status_text = "Internal Server Error"; break;
    case 501: status_text = "Not Implemented"; break;
    case 505: status_text = "Version Not Supported"; break;
    default:
        status_text = "Internal Server Error";
        code = 500;
        break;
    }
    // Build the body: "<Status-Phrase>\n"
    char body_buf[64];
    size_t body_len = (size_t) snprintf(body_buf, sizeof(body_buf), "%s\n", status_text);

    char header[256];
    size_t h_len = (size_t) snprintf(header, sizeof(header),
        "HTTP/1.1 %d %s\r\n"
        "Content-Length: %zu\r\n"
        "\r\n",
        code, status_text, body_len);

    write_n_bytes(client_fd, header, h_len);
    write_n_bytes(client_fd, body_buf, body_len);
}

void httpsuccess_header(int client_fd, int code, size_t content_length) {
    const char *status_text;

    if (code == 201) {
        status_text = "Created";
    } else { // default to 200/OK
        code = 200;
        status_text = "OK";
    }

    char header[256];
    int hlen = snprintf(header, sizeof(header),
        "HTTP/1.1 %d %s\r\n"
        "Content-Length: %zu\r\n"
        "\r\n",
        code, status_text, content_length);

    write_n_bytes(client_fd, header, (size_t) hlen);
}

static rwlock_t *get_uri_lock(const char *uri) {
    ENTRY e, *ep;
    rwlock_t *result_rw = NULL;
    pthread_mutex_lock(&uri_hash_lock); // lock the hash table
    pthread_cleanup_push(clean_up_mutex_unlock, &uri_hash_lock); // cleanup handler

    e.key = (char *) uri; // use the uri as the key
    e.data = NULL; // no data yet
    ep = hsearch(e, FIND); // search for the key
    if (ep != NULL) { // Lock found in hash table
        result_rw = (rwlock_t *) ep->data;
        if (result_rw == NULL) {
            fprintf(
                debug_log_fp, "Error: Found URI entry for '%s' but its lock data is NULL.\n", uri);
        }
    } else { // Lock not found, attempt to create and enter a new one
        char *uri_copy = strdup(uri);
        if (uri_copy == NULL) {
            fprintf(debug_log_fp, "Error: strdup failed for URI '%s' in get_uri_lock.\n", uri);
        } else {
            rwlock_t *new_rwlock = rwlock_new(WRITERS, 0); // Assuming READERS priority
            if (new_rwlock == NULL) {
                fprintf(
                    debug_log_fp, "Error: rwlock_new failed for URI '%s' in get_uri_lock.\n", uri);
                free(uri_copy); // Clean up allocated uri_copy
            } else {
                ENTRY new_entry = { .key = uri_copy, .data = new_rwlock };
                ENTRY *ep_entered = hsearch(new_entry, ENTER); // Attempt to add to hash table

                if (ep_entered == NULL) {
                    // fprintf(
                    // stderr, "Error: hsearch ENTER failed for URI '%s' in get_uri_lock.\n", uri);
                    rwlock_delete(&new_rwlock);
                    free(uri_copy);
                } else {
                    result_rw = (rwlock_t *) ep_entered->data; // Or simply new_rwlock
                }
            }
        }
    }

    // If, after all attempts, the_lock_to_return is still NULL,
    // it indicates a failure at some point.
    if (result_rw == NULL) {
        fprintf(debug_log_fp, "Ultimately failed to get or create rwlock for URI: '%s'.\n", uri);
    }

    pthread_cleanup_pop(1); // This executes clean_up_mutex_unlock(&uri_hash_lock)
    return result_rw;
}

void handle_connection(int client_fd) {
    // first we read in the request line, and header-field-list, followed by an empty line
    // initialize buffer for reading headers
    char header_buf[MAX_HEADER_LEN + 1];
    size_t header_len = 0;
    int header_read = 0;

    // Read one byte at a time until we see the header terminator \r\n\r\n
    while (!header_read && header_len < MAX_HEADER_LEN) {
        ssize_t r = read(client_fd, header_buf + header_len, 1);
        if (r <= 0) {
            // Client closed connection or read error or eof
            break;
        }
        header_len += (size_t) r;

        // if we've read at least 4 bytes
        // compare the last 4 bytes to \r\n\r\n
        // if they match, we have a complete header and can exit our loop
        if (header_len >= 4 && memcmp(header_buf + header_len - 4, "\r\n\r\n", 4) == 0) {
            header_read = 1;
        }
    }

    // No header read or too large -> respond with error
    if (header_len == 0 || header_len >= MAX_HEADER_LEN || !header_read) {
        httperror(client_fd, 400);
        add_to_log("INVALID", "INVALID", 400, "0");
        close(client_fd);
        return;
    }

    // Null-terminate header for string operations
    header_buf[header_len] = '\0';

    // Separate request-line from header-field-list
    char *line_end = strstr(header_buf, "\r\n"); // point to end of request line
    if (!line_end) {
        // bad request-line
        httperror(client_fd, 400);
        add_to_log("INVALID", "INVALID", 400, "0");
        close(client_fd);
        return;
    }
    // the length of the request line is the distance from the start of the buffer to the end of the request line
    size_t rl_len = (size_t) (line_end - header_buf);
    // store the request line in a separate buffer
    char request_line[rl_len + 1];
    memcpy(request_line, header_buf, rl_len);

    // Null-terminate the request line
    request_line[rl_len] = '\0';

    // Use regex to validate and capture method, URI, and HTTP version
    regex_t regex;
    // Build regex pattern : "^(method) (uri) (version)$"
    char rl_pattern[256];
    snprintf(rl_pattern, sizeof(rl_pattern), "^(%s) (%s) (%s)$", TYPE_REGEX, URI_REGEX, HTTP_REGEX);
    // Compile regex-pattern
    if (regcomp(&regex, rl_pattern, REG_EXTENDED) != 0) {
        fprintf(debug_log_fp, "Failed to compile regex\n");
        httperror(client_fd, 500);
        add_to_log("INVALID", "INVALID", 500, "0");
        close(client_fd);
        return;
    }

    regmatch_t pmatch[4];
    // pmatch[0] is the entire match, pmatch[1] is method, pmatch[2] is uri, pmatch[3] is version
    if (regexec(&regex, request_line, 4, pmatch, 0) != 0) { // run regex matching
        regfree(&regex);
        // Invalid request-line format
        httperror(client_fd, 400);
        add_to_log("INVALID", "INVALID", 400, "0");
        close(client_fd);
        return;
    }
    // Extract captured groups: method, uri, version
    char method[9], uri[64], version[16]; // init buffers for method, uri, and version
    // Copy the captured groups into the buffers

    //method:
    size_t method_len
        = (size_t) (pmatch[1].rm_eo - pmatch[1].rm_so); // calculate length of method (end-start)
    memcpy(method, request_line + pmatch[1].rm_so, method_len); // save to buffer
    method[method_len] = '\0'; // null-terminate

    //uri:
    size_t uri_len = (size_t) (pmatch[2].rm_eo - pmatch[2].rm_so);
    memcpy(uri, request_line + pmatch[2].rm_so, uri_len);
    uri[uri_len] = '\0';

    //version:
    size_t ver_len = (size_t) (pmatch[3].rm_eo - pmatch[3].rm_so);
    memcpy(version, request_line + pmatch[3].rm_so, ver_len);
    version[ver_len] = '\0';

    // clean up regex
    regfree(&regex);

    // Only support HTTP/1.1
    if (strcmp(version, HTTP_VERSION) != 0) {
        httperror(client_fd, 505);
        add_to_log("INVALID", "INVALID", 505, "0");
        close(client_fd);
        return;
    }

    size_t content_length = 0;

    char *head_start = line_end + 2; // point to start of header-field-list
    char *head_end = header_buf + header_len;
    char request_id[128] = "0";
    bool expect_continue = false;
    // iterate line by line through header-field-list
    for (char *p = head_start; p < head_end - 2; p = strchr(p, '\n') + 1) {
        // Find end of line
        char *eol = strstr(p, "\r\n");
        if (!eol)
            continue;
        // find size of line
        size_t l_len = (size_t) (eol - p);
        char line[256];
        if (l_len >= sizeof(line))
            continue; // line too long skip but still check for other headers for logging
        // Copy line into buffer
        memcpy(line, p, l_len);
        line[l_len] = '\0';

        // line should be of the form "key: value\r\"

        // Parse key: value
        char key[129], value[129];
        if (sscanf(line, "%128[^:]: %128[^\"]", key, value) == 2) {
            if (strcasecmp(key, "Content-Length") == 0) {
                content_length = strtoul(value, NULL, 10);
            } else if (strcasecmp(key, "Request-Id") == 0) {
                strncpy(request_id, value, sizeof(request_id) - 1);
                request_id[sizeof(request_id) - 1] = '\0'; // ensure null termination
            } else if (strcasecmp(key, "Expect") == 0) {
                if (strcasecmp(value, "100-continue") == 0) {
                    expect_continue = true;
                }
            }
        }
    }

    int ret_code; // save return code for logging
    if (strcmp(method, "PUT") == 0) {
        if (expect_continue) {
            // Send 100-continue response
            write_n_bytes(client_fd, "HTTP/1.1 100 Continue\r\n\r\n",
                strlen("HTTP/1.1 100 Continue\r\n\r\n"));
        }
        // The modified handle_put_request will now manage its own lock acquisition.
        ret_code = handle_put_request(client_fd, uri, content_length);

    } else if (strcmp(method, "GET") == 0) {
        rwlock_t *rw_lock = get_uri_lock(uri); // GET still needs lock managed here
        if (rw_lock == NULL) {
            httperror(client_fd, 500);
            add_to_log(method, uri, 500, request_id); // Use actual method and uri for logging
            close(client_fd);
            return;
        }
        pthread_cleanup_push(clean_up_reader_unlock, rw_lock);
        reader_lock(rw_lock);
        ret_code = handle_get_request(client_fd, uri); // Your existing GET handler
        reader_unlock(rw_lock);
        pthread_cleanup_pop(0);
    } else {
        // Unsupported method
        httperror(client_fd, 501);
        ret_code = 501;
    }

    // log the request
    add_to_log(method, uri, ret_code, request_id);

    // eat the junk bytes at the end of the request
    char junk[BUFFER_SIZE];
    while (read(client_fd, junk, sizeof(junk)) > 0) {
        // do nothing
    }
    close(client_fd);
}

int handle_put_request(int client_fd, const char *uri, size_t content_length) {
    char temp_filename_template[] = "/tmp/http_put_XXXXXX"; // Template for mkstemp
    int temp_fd = mkstemp(temp_filename_template);

    if (temp_fd == -1) {
        if (debug_log_fp) {
            fprintf(
                debug_log_fp, "ERROR_PUT: mkstemp() failed for URI %s: %s\n", uri, strerror(errno));
            fflush(debug_log_fp);
        }
        httperror(client_fd, 500);
        return 500;
    }

    if (debug_log_fp) {
        fprintf(debug_log_fp,
            "DEBUG_PUT_START: URI=%s, client_fd=%d, temp_fd=%d (file: %s), "
            "expected_content_length=%zu. Starting body read.\n",
            uri, client_fd, temp_fd, temp_filename_template, content_length);
        fflush(debug_log_fp);
    }

    // --- Phase 1: Receive request body into the temporary file ---
    size_t total_bytes_written_to_temp = 0;
    char body_read_buffer[BUFFER_SIZE];
    bool io_error_occurred = false; // Tracks if any read/write error happened

    if (content_length > 0) {
        while (total_bytes_written_to_temp < content_length) {
            size_t bytes_to_read_now = content_length - total_bytes_written_to_temp;
            if (bytes_to_read_now > BUFFER_SIZE) {
                bytes_to_read_now = BUFFER_SIZE;
            }

            ssize_t bytes_read_from_client = read(client_fd, body_read_buffer, bytes_to_read_now);

            if (bytes_read_from_client < 0) { // Read error from client
                if (errno == EINTR)
                    continue; // Interrupted, retry read
                if (debug_log_fp) {
                    fprintf(debug_log_fp,
                        "DEBUG_PUT_READ_ERROR: URI=%s, read from client_fd failed: %s\n", uri,
                        strerror(errno));
                    fflush(debug_log_fp);
                }
                io_error_occurred = true;
                break;
            }
            if (bytes_read_from_client == 0) { // EOF from client
                if (debug_log_fp) {
                    fprintf(debug_log_fp,
                        "DEBUG_PUT_READ_EOF: URI=%s, client closed connection. Expected %zu, got "
                        "%zu.\n",
                        uri, content_length, total_bytes_written_to_temp);
                    fflush(debug_log_fp);
                }
                break; // Will lead to 400 if total_bytes != content_length
            }

            ssize_t bytes_written_to_fd
                = write_n_bytes(temp_fd, body_read_buffer, (size_t) bytes_read_from_client);
            if (bytes_written_to_fd != bytes_read_from_client) { // Error writing to temp file
                if (debug_log_fp) {
                    fprintf(debug_log_fp,
                        "DEBUG_PUT_WRITE_ERROR: URI=%s, write_n_bytes to temp_fd failed or "
                        "incomplete. Attempted %zd, wrote %zd. Errno: %s\n",
                        uri, bytes_read_from_client, bytes_written_to_fd, strerror(errno));
                    fflush(debug_log_fp);
                }
                io_error_occurred = true;
                break;
            }
            total_bytes_written_to_temp += (size_t) bytes_written_to_fd;
        }
    }
    // For content_length == 0, total_bytes_written_to_temp remains 0, loop is skipped.

    if (debug_log_fp) {
        fprintf(debug_log_fp,
            "DEBUG_PUT_BODY_READ_END: URI=%s, expected_len=%zu, actual_len=%zu, io_error=%s\n", uri,
            content_length, total_bytes_written_to_temp, io_error_occurred ? "yes" : "no");
        fflush(debug_log_fp);
    }

    // Check if all content was received or if an I/O error occurred
    if (io_error_occurred) {
        close(temp_fd);
        unlink(temp_filename_template);
        httperror(client_fd, 500); // Server-side I/O error or client read error
        return 500;
    }
    if (total_bytes_written_to_temp
        != content_length) { // Client sent less data than Content-Length
        close(temp_fd);
        unlink(temp_filename_template);
        httperror(client_fd, 400); // Bad Request: Incomplete body
        return 400;
    }

    // Body fully received and written. Close the temporary file descriptor.
    if (close(temp_fd) == -1) {
        if (debug_log_fp) {
            fprintf(debug_log_fp, "ERROR_PUT: Failed to close temporary file %s for URI %s: %s\n",
                temp_filename_template, uri, strerror(errno));
            fflush(debug_log_fp);
        }
        unlink(temp_filename_template); // Attempt to clean up
        httperror(client_fd, 500);
        return 500;
    }

    // --- Phase 2: Critical Section (Lock, Stat, Rename, Chmod, Unlock) ---
    const char *filename = uri + 1; // Assumes URI like "/file.txt"
    int status_code = 500; // Default to an internal server error for this phase
    bool file_existed_before_rename;

    rwlock_t *rw_lock = get_uri_lock(uri);
    if (rw_lock == NULL) {
        if (debug_log_fp) {
            fprintf(debug_log_fp,
                "ERROR_PUT: Failed to get/create URI lock for PUT on %s. Temp file %s not "
                "committed.\n",
                uri, temp_filename_template);
            fflush(debug_log_fp);
        }
        unlink(temp_filename_template); // Temp file was created but cannot be committed
        httperror(client_fd, 500);
        return 500;
    }

    pthread_cleanup_push(clean_up_writer_unlock, rw_lock);
    writer_lock(rw_lock);

    struct stat st_check;
    file_existed_before_rename = (stat(filename, &st_check) == 0);

    if (rename(temp_filename_template, filename) == 0) {
        if (chmod(filename, 0644) == -1) {
            if (debug_log_fp) {
                fprintf(debug_log_fp, "WARN_PUT: chmod(0644) on %s failed after rename: %s\n",
                    filename, strerror(errno));
                fflush(debug_log_fp);
            }
            // This is a warning; the file was still renamed. The PUT can be considered successful.
        }

        if (file_existed_before_rename) {
            httpsuccess_header(client_fd, 200, 3); // "OK\n"
            write_n_bytes(client_fd, "OK\n", 3);
            status_code = 200;
        } else {
            httpsuccess_header(client_fd, 201, 8); // "Created\n"
            write_n_bytes(client_fd, "Created\n", 8);
            status_code = 201;
        }
    } else {
        // rename() failed. The temporary file still exists and must be unlinked.
        if (debug_log_fp) {
            fprintf(debug_log_fp,
                "ERROR_PUT: Renaming temporary file '%s' to target '%s' for URI %s failed: %s\n",
                temp_filename_template, filename, uri, strerror(errno));
            fflush(debug_log_fp);
        }
        unlink(temp_filename_template);

        if (errno == EACCES || errno == EPERM) {
            httperror(client_fd, 403); // Forbidden
            status_code = 403;
        } else if (errno == EISDIR || errno == ENOTDIR || errno == ENOENT) {
            // ENOENT could mean path to filename is bad, or filename is invalid type for rename target
            httperror(client_fd, 403); // Often maps to Forbidden or Not Found
            status_code = 403;
        } else {
            httperror(client_fd, 500); // Other internal server error
            status_code = 500;
        }
    }

    writer_unlock(rw_lock);
    pthread_cleanup_pop(0);

    return status_code;
}

int handle_get_request(int client_fd, const char *uri) {
    // parse uri to get the filename
    const char *filename = uri + 1; // skip leading '/'
    struct stat st;

    // verify file permissions and existence
    if (stat(filename, &st) < 0) {
        if (errno == ENOENT) { // 404 not found
            httperror(client_fd, 404);
            return 404;
        } else {
            // other error
            httperror(client_fd, 403);
            return 403;
        }
    }
    if (!S_ISREG(st.st_mode) || access(filename, R_OK) < 0) {
        httperror(client_fd, 403);
        return 403;
    }

    // open the file for reading
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        httperror(client_fd, 404);
        return 404;
    }

    // print header
    httpsuccess_header(client_fd, 200, (size_t) st.st_size);

    char buf[BUFFER_SIZE];
    ssize_t n;
    while ((n = read(fd, buf, sizeof(buf))) > 0) {
        if (write_n_bytes(client_fd, buf, (size_t) n) < 0) {
            break; // client disconnected
        }
    }

    close(fd);
    return 200;
}

void *worker_thread(void *arg) {
    (void) arg; // suppress unused variable warning
    while (1) { // thread waits for task
        int *client_fd_ptr = NULL;

        // pop task from queue
        if (queue_pop(work_queue, (void **) &client_fd_ptr)) {
            if (client_fd_ptr != NULL) { // null check for saftey
                int client_fd = *client_fd_ptr;
                free(client_fd_ptr); // free the pointer instantly now that we have the fd
                handle_connection(client_fd); // handle the connection
            }
        }
    }
    return NULL;
}

void add_to_log(const char *method, const char *uri, int status_code, const char *request_id) {
    // Lock the audit log mutex
    pthread_mutex_lock(&audit_log_lock);
    pthread_cleanup_push(clean_up_mutex_unlock, &audit_log_lock); // cleanup handler

    fprintf(stderr, "%s,%s,%d,%s\n", method, uri, status_code, request_id);
    fflush(stderr); // flush the log to ensure it's written immediately

    pthread_cleanup_pop(1); // pop the cleanup handler
}
