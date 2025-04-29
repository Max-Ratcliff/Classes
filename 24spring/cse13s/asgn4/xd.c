#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
    int fd = 0;

    if (argc == 1) {
        // $ readfile
        fd = 0;
    } else if (argc == 2) {
        // $ readfile filename
        char *filename = argv[1];

        fd = open(filename, O_RDONLY);

        if (fd == -1) {
            //printf("Can't open file %s\n", filename);
            exit(1);
        }
    } else {
        exit(1);
    }

    ssize_t res = 0;
    int line_count = 0;
    do {
        unsigned char buffer[16]; //initialize buffer to be 16 characters long
        res = read(fd, buffer, 16); //exit codes: -1 =  error, 0 = EOF
        if (res == -1) {
            //printf("Error during read.\n");
            exit(1);
        } else if (res > 0) {
            printf("%08x: ", line_count); //leftmost column printing index of the first byte
            while (res < 16) {
                ssize_t new_res = read(fd, buffer + res, (size_t) (16 - res));
                if (new_res > 0) {
                    res += new_res;
                } else {
                    break;
                }
            }
            for (int i = 0; i < 16; i++) { //middle column prints 16 bytes of the file in hex
                if (i < res) {
                    printf("%02x", buffer[i]);
                } else {
                    printf("  "); // 2 white spaces
                }
                if (i + 1 < res) {
                    printf("%02x ", buffer[i + 1]);
                    i++;

                } else {
                    printf("   "); //3 white spaces
                    i++;
                }
            }
            printf(" ");
            for (int i = 0; i < res; ++i) { // rightmost column prints 16 bytes of the file in ascii
                printf("%c", isprint(buffer[i]) ? buffer[i] : '.');
            }
            printf("\n");
        } else {
            //EOF
        }
        line_count += 16;
    } while (res > 0);

    res = close(fd);

    if (res == -1) {
        //printf("Error during close.\n");
        exit(1);
    }

    return 0;
}
