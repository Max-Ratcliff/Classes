#include "cse13s_random.h"
#include "names.h"

#include <stdio.h>

typedef enum { DOT, LEFT, CENTER, RIGHT } Position;

//takes seed as input and returns dice roll
Position roll_dice(void) {
    const Position die[6] = {
        DOT,
        DOT,
        DOT,
        LEFT,
        CENTER,
        RIGHT,
    };

    int die_val = cse13s_random() % 6; //should produce a 0-5 int
    return die[die_val];
}

int no_winner(int chips[], int num_players) {
    int has_chip = 0;
    int last_player_with_chip = -1;
    for (int i = 0; i < num_players; i++) {
        if (chips[i] > 0 && has_chip) {
            return 1;
        }
        if (chips[i] > 0) {
            has_chip = 1;
            last_player_with_chip = i;
        }
    }
    printf("%s won!\n", player_name[last_player_with_chip]);
    return 0;
}

void initchips(int chips[], int num_players) {
    for (int i = 0; i < num_players; i++) {
        chips[i] = 3;
    }
}

void sim_game(int num_players) {
    int chips[num_players];
    initchips(chips, num_players);
    int roll;
    int loop = 1;
    while (loop) { //loop until winner
        for (int i = 0; i < num_players; i++) {
            if (chips[i] > 0) {
                int num_rolls = chips[i];
                if (num_rolls >= 3) {
                    num_rolls = 3;
                }
                for (int j = 0; j < num_rolls; j++) {
                    roll = roll_dice();
                    if (roll == LEFT) {
                        chips[i]--;
                        if (i == num_players - 1) {
                            chips[0]++;
                        } else {
                            chips[i + 1]++;
                        }
                    } else if (roll == CENTER) {
                        chips[i]--;
                    } else if (roll == RIGHT) {
                        chips[i]--;
                        if (i == 0) {
                            chips[num_players - 1]++;
                        } else {
                            chips[i - 1]++;
                        }
                    }
                }
                printf("%s: ends her turn with %d\n", player_name[i], chips[i]);
                if (!no_winner(chips, num_players)) {
                    loop = 0;
                    break;
                }
            }
        }
    }
}

int main(void) {
    //get number of players from user
    int num_players = 3;
    printf("Number of players (3 to 11)? ");
    int scanf_result = scanf("%d", &num_players);

    //check input for errors
    if (scanf_result < 1 || num_players < 3 || num_players > MAX_PLAYERS) {
        fprintf(stderr, "Invalid number of players. Using 3 instead.\n");
        num_players = 3;
    }

    //get seed from user
    unsigned seed = 4823;
    printf("Random-number seed? ");
    scanf_result = scanf("%u", &seed);

    //check input for errors
    if (scanf_result < 1) {
        fprintf(stderr, "Invalid seed. Using 4823 instead.\n");
    }

    //set seed for PRNG
    cse13s_random_seed(seed);
    //run the LCR game
    sim_game(num_players);

    return 0;
}
