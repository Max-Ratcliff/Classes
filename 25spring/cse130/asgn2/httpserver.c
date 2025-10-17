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
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "iowrapper.h"
#include "protocol.h"
#include "listener_socket.h"

#define BUFFER_SIZE 4096
#define PATH_MAX    4096

// method shortcuts for readability
#define PUT     0
#define GET     1
#define INVALID -1

// HTTP return headers implemented under main
void httperror(int client_fd, int code);
void httpsuccess_header(int client_fd, int code, size_t content_length);
// handle PUT request
int handle_put_request(int client_fd, const char *uri, size_t content_length);
// handle GET request
int handle_get_request(int client_fd, const char *uri);

static Listener_Socket_t *listen_socket = NULL;
static void on_shutdown(int sig) {
    (void) sig; // suppress unused variable warning
    // Clean up the listener socket
    if (listen_socket != NULL) {
        ls_delete(&listen_socket);
        listen_socket = NULL;
    }
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port between 1-65535>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Check if the port is a valid number
    long port_long = strtol(argv[1], NULL, 10);
    if (port_long <= 0 || port_long > 65535) {
        fprintf(stderr, "Invalid Port\n");
        return EXIT_FAILURE;
    }
    uint16_t port = (uint16_t) port_long;

    signal(SIGINT, on_shutdown); // handle SIGINT to clean up
    signal(SIGTERM, on_shutdown); // handle SIGTERM to clean up
    signal(SIGPIPE, SIG_IGN); // ignore SIGPIPE signal

    // Create the socket
    listen_socket = ls_new(port);
    if (listen_socket == NULL) {
        fprintf(stderr, "Listening: Invalid Port\n");
        return EXIT_FAILURE;
    }

    printf("Server listening on port %d\n", port); // DEBUG

    // begin listening
    while (1) {
        int client_fd = ls_accept(listen_socket);
        if (client_fd < 0) {
            fprintf(stderr, "Failed to accept connection\n");
            continue; // Skip to the next iteration
        }

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
            close(client_fd);
            continue;
        }

        // Null-terminate header for string operations
        header_buf[header_len] = '\0';

        // Separate request-line from header-field-list
        char *line_end = strstr(header_buf, "\r\n"); // point to end of request line
        if (!line_end) {
            // bad request-line
            httperror(client_fd, 400);
            close(client_fd);
            continue;
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
        snprintf(
            rl_pattern, sizeof(rl_pattern), "^(%s) (%s) (%s)$", TYPE_REGEX, URI_REGEX, HTTP_REGEX);
        // Compile regex-pattern
        if (regcomp(&regex, rl_pattern, REG_EXTENDED) != 0) {
            fprintf(stderr, "Failed to compile regex\n");
            httperror(client_fd, 500);
            close(client_fd);
            continue;
        }

        regmatch_t pmatch[4];
        // pmatch[0] is the entire match, pmatch[1] is method, pmatch[2] is uri, pmatch[3] is version
        if (regexec(&regex, request_line, 4, pmatch, 0) != 0) { // run regex matching
            regfree(&regex);
            // Invalid request-line format
            httperror(client_fd, 400);
            close(client_fd);
            continue;
        }
        // Extract captured groups: method, uri, version
        char method[9], uri[64], version[16]; // init buffers for method, uri, and version
        // Copy the captured groups into the buffers

        //method:
        size_t method_len = (size_t) (pmatch[1].rm_eo
                                      - pmatch[1].rm_so); // calculate length of method (end-start)
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
            close(client_fd);
            continue;
        }

        size_t content_length = 0;

        // execute request
        // check if method is PUT or GET
        // otherwise return 501 for now "Not Implemented"
        if (strcmp(method, "PUT") == 0) {
            char *head_start = line_end + 2; // point to start of header-field-list
            char *head_end = header_buf + header_len;
            // iterate line by line through header-field-list
            for (char *p = head_start; p < head_end - 2; p = strchr(p, '\n') + 1) {
                // Find end of line
                char *eol = strstr(p, "\r\n");
                if (!eol)
                    break;
                // find size of line
                size_t l_len = (size_t) (eol - p);
                char line[256];
                if (l_len >= sizeof(line))
                    break;
                // Copy line into buffer
                memcpy(line, p, l_len);
                line[l_len] = '\0';

                // line should be of the form "key: value\r\"

                // Parse key: value
                char key[129], value[129];
                if (sscanf(line, "%128[^:]: %128[^\"]", key, value) == 2) {
                    if (strcasecmp(key, "Content-Length") == 0) {
                        content_length = strtoul(value, NULL, 10);
                    }
                }
            }
            // PUT requires a Content-Length header
            if (content_length == 0) {
                httperror(client_fd, 400);
                close(client_fd);
                continue;
            }

            if (handle_put_request(client_fd, uri, content_length) < 0) {
                // Error handling in handle_put_request
                close(client_fd);
                continue;
            }

        } else if (strcmp(method, "GET") == 0) {
            if (handle_get_request(client_fd, uri) < 0) {
                // Error handling in handle_get_request
                close(client_fd);
                continue;
            }
        } else {
            // Unsupported method
            httperror(client_fd, 501);
            close(client_fd);
            continue;
        }

        // eat the junk bytes at the end of the request
        char junk[BUFFER_SIZE];
        while (read(client_fd, junk, sizeof(junk)) > 0) {
            // do nothing
        }
        // close the client socket
        close(client_fd);
    }

    // Clean up and return
    ls_delete(&listen_socket);
    if (listen_socket != NULL) {
        fprintf(stderr, "Failed to delete listener socket\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

void httperror(int client_fd, int code) {
    const char *status_text;

    if (code == 400) {
        status_text = "Bad Request";
    } else if (code == 403) {
        status_text = "Forbidden";
    } else if (code == 404) {
        status_text = "Not Found";
    } else if (code == 500) {
        status_text = "Internal Server Error";
    } else if (code == 501) {
        status_text = "Not Implemented";
    } else if (code == 505) {
        status_text = "Version Not Supported";
    } else {
        status_text = "Internal Server Error";
        code = 500;
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

    if (code == 200) {
        status_text = "OK";
    } else if (code == 201) {
        status_text = "Created";
    } else {
        // default to 200/OK
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

int handle_put_request(int client_fd, const char *uri, size_t content_length) {
    // allocate memory for the message body
    char *body = malloc(content_length);
    if (!body) {
        httperror(client_fd, 500);
        return -1;
    }

    // read the message body from the client
    ssize_t bytes_read = read_n_bytes(client_fd, body, content_length);
    if (bytes_read < 0 || (size_t) bytes_read != content_length) {
        free(body);
        httperror(client_fd, 500);
        return -1;
    }

    // parse uri to get the filename
    const char *filename = uri + 1; // skip leading '/'
    int file_exists = access(filename, F_OK) == 0;

    // open the file for writing
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        free(body);
        httperror(client_fd, 403);
        return -1;
    }

    // write the message body to the file
    ssize_t bytes_written = write(fd, body, content_length);
    close(fd);
    free(body);

    if (bytes_written < 0 || (size_t) bytes_written != content_length) {
        httperror(client_fd, 500);
        return -1;
    }

    // success response
    if (file_exists) {
        httpsuccess_header(client_fd, 200, 3); // 3 bytes for "OK\n"
        write_n_bytes(client_fd, "OK\n", 3);
    } else {
        httpsuccess_header(client_fd, 201, 8); // 8 bytes for "Created\n"
        write_n_bytes(client_fd, "Created\n", 8);
    }

    return 0;
}

int handle_get_request(int client_fd, const char *uri) {
    // parse uri to get the filename
    const char *filename = uri + 1; // skip leading '/'
    struct stat st;

    // verify file permissions and existence
    if (stat(filename, &st) < 0) {
        httperror(client_fd, errno == ENOENT ? 404 : 403);
        return -1;
    }
    if (!S_ISREG(st.st_mode) || access(filename, R_OK) < 0) {
        httperror(client_fd, 403);
        return -1;
    }

    // open the file for reading
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        httperror(client_fd, 404);
        return -1;
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
    return 0;
}
