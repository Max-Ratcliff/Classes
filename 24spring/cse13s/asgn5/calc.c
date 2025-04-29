#include "mathlib.h"
#include "messages.h"
#include "operators.h"
#include "stack.h"

#include <assert.h>
#include <ctype.h>
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
    bool m_flag = false;
    int opt = getopt(argc, argv, "hm");
    if (opt == 'h') {
        printf("%s", USAGE);
    }
    if (opt == 'm') {
        m_flag = true;
    }

    while (true) {
        fprintf(stderr, "> ");
        char expr[1024];

        char *saveptr;
        if (fgets(expr, 1024, stdin) == NULL) {
            break;
        }

        int character;
        bool error = false;
        const char *token = strtok_r(expr, " ", &saveptr);

        while (token != NULL && !error) {
            double val;
            character = (int) *token;
            if (parse_double(token, &val)) {
                if (!stack_push(val)) {
                    fprintf(stderr, ERROR_NO_SPACE, val);
                    error = true;
                    break;
                }
            } else if (*binary_operators[character] != NULL) {
                if (!apply_binary_operator(*binary_operators[character])) {
                    fprintf(stderr, ERROR_BINARY_OPERATOR);
                    error = true;
                    break;
                }
            } else if (!m_flag && *my_unary_operators[character] != NULL) {
                if (!apply_unary_operator(*my_unary_operators[character])) {
                    fprintf(stderr, ERROR_UNARY_OPERATOR);
                    error = true;
                    break;
                }
            } else if (m_flag && *libm_unary_operators[character] != NULL) {
                if (!apply_unary_operator(*libm_unary_operators[character])) {
                    fprintf(stderr, ERROR_UNARY_OPERATOR);
                    error = true;
                    break;
                }
            } else {
                fprintf(stderr, ERROR_BAD_CHAR, *token);
                error = true;
                break;
            }

            token = strtok_r(NULL, " ", &saveptr);
        }
        if (!error) {
            stack_print();
            printf("\n");
        }
        stack_clear();
    }
    return 0;
}
