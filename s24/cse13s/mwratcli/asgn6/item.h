#ifndef _ITEM
#define _ITEM

#include <stdbool.h>

typedef struct item {
    char key[255];
    int id;
} item;

bool cmp(item *, item *);

#endif
