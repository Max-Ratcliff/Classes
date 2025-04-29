#include "hangman_helpers.h"

bool is_lowercase_letter(char c) {
    if (((int) c < 97) || ((int) c > 122)) {
        return false;
    }
    return true;
}

bool is_valid_secret(const char *secret) {
    for (int i = 0; i <= 256; i++) { //loop from start to end of possible string length
        if (is_lowercase_letter(secret[i])) { //if character is a valid letter between a-z
            continue;
        } else if ((int) secret[i] == 32 || (int) secret[i] == 39
                   || (int) secret[i] == 45) { //if character is an accepted special
            continue;
        } else if ((int) secret[i] == 0) {
            return true;
        }
        printf("invalid character: '%c'\n", secret[i]);
        printf("the secret phrase must contain only lowercase letters, spaces, hyphens, and "
               "apostrophes\n");
        return false;
    }
    printf("the secret phrase is over 256 characters\n");
    return false; //valid secret
}

bool string_contains_character(const char *s, char c) {
    for (int i = 0; i < 256; i++) { //loop from start to end of possible string length
        if (s[i] == c) { //if character at index is character that we're looking for return true
            return true;
        } else if ((int) s[i] == 0) { //if character at index is null terminator
            return false;
        }
    }
    return false; //string too long
}

char read_letter(void) {
    char letter;

    printf("Guess a letter: ");
    scanf("%c", &letter);
    getchar();

    return letter;
}
