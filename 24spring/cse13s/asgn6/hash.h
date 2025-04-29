#ifndef _HASH

#define _HASH
#define BUCKETS 1000

#include "ll.h"

typedef struct Hashtable Hashtable;

struct Hashtable {
    LL *bucket[BUCKETS];
};

Hashtable *hash_create(void);

bool hash_put(Hashtable *, char *key, int val);

int *hash_get(Hashtable *, char *key);

void hash_destroy(Hashtable **);

#endif
