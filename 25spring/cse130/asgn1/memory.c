#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/stat.h>

# define PATH_MAX 4096

// Helper to read a line from file descriptor 'fd' until '\n' or bufsize-1 characters are read.
// we read each line 1 character at a time so we can detect the end of line. and properly null-terminate our input.
// returns -1 on error, 0 on EOF, or the number of bytes read on success.
ssize_t read_line(int fd, char *buffer, size_t bufsize) {
    size_t i = 0;
    char c;
    ssize_t n;
    bool newline_found = false;
    while (i < bufsize - 1) {
        n = read(fd, &c, 1);
        if (n < 0) {
            return -1; // error reading
        } else if (n == 0) {
            break; // EOF reached
        }
        if (c == '\n') {
            newline_found = true;
            break; // end of line reached properly
        }
        buffer[i++] = c;
    }
    buffer[i] = '\0';

    // Ensure that header lines are terminated by a newline
    if (!newline_found) {
        return -1; // indicate error if no newline was encountered
    }

    return i;
}

int main(void) {
    char headerBuffer[PATH_MAX];
    int ret;

    // get command
    ret = read_line(STDIN_FILENO, headerBuffer, sizeof(headerBuffer));
    if (ret <= 0) {
        fprintf(stderr, "Invalid Command\n");
        return 1;
    }
    char command[16]; // Enough to hold "get" or "set"
    strncpy(command, headerBuffer, sizeof(command) - 1);
    command[sizeof(command) - 1] = '\0';

    if (strcmp(command, "get") != 0 && strcmp(command, "set") != 0) {
        fprintf(stderr, "Invalid Command\n");
        return 1;
    }

    // get filepath
    ret = read_line(STDIN_FILENO, headerBuffer, sizeof(headerBuffer));
    if (ret <= 0) {
        fprintf(stderr, "Invalid Command\n");
        return 1;
    }
    char filename[PATH_MAX];
    strncpy(filename, headerBuffer, sizeof(filename) - 1);
    filename[sizeof(filename) - 1] = '\0';

    // ensure filename is null-terminated and within bounds
    if (strlen(filename) >= PATH_MAX) {
        fprintf(stderr, "Invalid Command\n");
        return 1;
    }

    // branch into get/set
    if (strcmp(command, "get") == 0) {
        // check for extra input by reading in O_NONBLOCK mode
        int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
        if (flags == -1)
            flags = 0;
        fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
        char dummy;
        ssize_t extra = read(STDIN_FILENO, &dummy, 1);
        fcntl(STDIN_FILENO, F_SETFL, flags); // restore flags
        if (extra > 0) {
            fprintf(stderr, "Invalid Command\n");
            return 1;
        }

        // Open the file for reading.
        int fd = open(filename, O_RDONLY);
        if (fd < 0) {
            if (errno == ENOENT)
                fprintf(stderr, "Invalid Command\n");
            else
                fprintf(stderr, "Operation Failed\n");
            return 1;
        }
        // Check that the file is not a directory.
        struct stat st;
        if (fstat(fd, &st) < 0) {
            fprintf(stderr, "Operation Failed\n");
            close(fd);
            return 1;
        }
        if (S_ISDIR(st.st_mode)) {
            fprintf(stderr, "Invalid Command\n");
            close(fd);
            return 1;
        }

        // Read from the file in chunks and write to STDOUT.
        char ioBuffer[4096];
        ssize_t n;
        while ((n = read(fd, ioBuffer, sizeof(ioBuffer))) > 0) {
            // attempt to write the data to STDOUT, if the write fails to write all bytes, we will try to write the remaining bytes
            ssize_t total_written = 0;
            while (total_written < n) {
                ssize_t written = write(STDOUT_FILENO, ioBuffer + total_written, n - total_written);
                if (written < 0) {
                    fprintf(stderr, "Operation Failed\n");
                    close(fd);
                    return 1;
                }
                total_written += written;
            }
        }
        if (n < 0) {
            fprintf(stderr, "Operation Failed\n");
            close(fd);
            return 1;
        }
        close(fd);
        return 0;
    } else { // "set" command
        // get the expected content length
        ret = read_line(STDIN_FILENO, headerBuffer, sizeof(headerBuffer));
        if (ret < 0) {
            fprintf(stderr, "Invalid Command\n");
            return 1;
        }
        // content length must be a positive integer
        int content_len = atoi(headerBuffer);
        if (content_len < 0) {
            fprintf(stderr, "Invalid Command\n");
            return 1;
        }

        // open file for writing
        // use flags to overwrite the file if it exists and create it if it doesn't exist
        int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) {
            fprintf(stderr, "Operation Failed\n");
            return 1;
        }

        // read the content from STDIN and write it to the file
        // read in chunks of 4096 bytes or until we reach content_len bytes
        char ioBuffer[4096];
        int total_read = 0;
        while (total_read < content_len) {
            int to_read = (content_len - total_read) < (int) sizeof(ioBuffer)
                              ? (content_len - total_read)
                              : sizeof(ioBuffer);
            ssize_t n = read(STDIN_FILENO, ioBuffer, to_read);
            if (n < 0) {
                fprintf(stderr, "Operation Failed\n");
                close(fd);
                return 1;
            }
            if (n == 0) {
                // EOF reached before reading content_len bytes
                // write out what was provided.
                break;
            }
            // buffered write to the file
            // to protect against partial writes, see the explanation in the get command
            int total_written = 0;
            while (total_written < n) {
                ssize_t written = write(fd, ioBuffer + total_written, n - total_written);
                if (written < 0) {
                    fprintf(stderr, "Operation Failed\n");
                    close(fd);
                    return 1;
                }
                total_written += written;
            }
            total_read += n;
        }
        close(fd);

        // print success message!
        // we cuold buffer this as well but its only 3 characters and its not worth 10 extra lines lol
        if (write(STDOUT_FILENO, "OK\n", 3) != 3) {
            fprintf(stderr, "Operation Failed\n");
            return 1;
        }
        return 0;
    }
}
