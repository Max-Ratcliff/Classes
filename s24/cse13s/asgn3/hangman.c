#include "hangman_helpers.h"

int main(int argc, char *argv[]) {
    int num_mistakes = 0;
    char guess = 'A';
    char mistakes[LOSING_MISTAKE] = { '_' };
    char *secret = argv[1];
    int secret_len = (int) strlen(argv[1]);
    char obfuscated[MAX_LENGTH] = { '_' };

    if (argc != 2) {
        printf("invalid argument quantity\n");
        return 1;
    }
    if (!is_valid_secret(argv[1])) {
        return 1;
    }

    while (num_mistakes <= LOSING_MISTAKE) {
        printf("%s\n", arts[num_mistakes]);
        for (int i = 0; i < secret_len; i++) {
            if (is_lowercase_letter(obfuscated[i])) {
                continue;

            } else if (secret[i] == guess || secret[i] == '\'' || secret[i] == ' '
                       || secret[i] == '-' || secret[i] == '\0') {
                obfuscated[i] = secret[i];
            } else {
                obfuscated[i] = '_';
            }
        }
        printf("    Phrase: %s\n", obfuscated);
        printf("Eliminated: ");
        for (int i = 97; i < 123; i++) {
            if (string_contains_character(mistakes, (char) i)) {
                printf("%c", (char) i);
            }
        }
        printf("\n\n");

        if (strcmp(obfuscated, secret) == 0) {
            printf("You win! The secret phrase was: %s\n", secret);
            break;
        }

        if (num_mistakes == LOSING_MISTAKE) {
            printf("You lose! The secret phrase was: %s\n", secret);
            break;
        }

        do {
            guess = (char) read_letter();
            for (int i = 0; i < LOSING_MISTAKE; i++) {
                if (guess == mistakes[i]) {
                    guess = 'A';
                    break;
                }
            }
        } while (!is_lowercase_letter(guess) || string_contains_character(obfuscated, guess));

        if (!string_contains_character(secret, guess)) {
            mistakes[num_mistakes] = guess;
            num_mistakes++;
        }
    }

    return 0;
}
