#include "hash.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    int counter = 0;
    Hashtable *h = hash_create();
    char buffy[1024];
    while (true) {

        if (!fgets(buffy, 1024, stdin)) {
            break;
        }
        if (!hash_get(h, buffy)) {
            hash_put(h, buffy, 1);
            counter++;
        }
    }
    printf("%d\n", counter);
    hash_destroy(&h);

    return 0;
}
