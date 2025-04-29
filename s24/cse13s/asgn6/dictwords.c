#include "hash.h"

#include <stdio.h>
#include <string.h>

void *abstractwords(int, void *, void (*dsdo)(void *, char *, int));

void hashdo(void *ds, char *line, int i) {
    Hashtable *h = (Hashtable *) ds;
    int *ret = hash_get(h, line);
    if (ret == NULL) {
        hash_put(h, line, i);
    }
}

void listdo(void *ds, char *line, int i) {
    LL *l = (LL *) ds;
    item it;
    strcpy(it.key, line);
    it.id = i;
    list_add(l, it);
}

Hashtable *uniqwords(int iters) {
    Hashtable *h = hash_create();
    return (Hashtable *) abstractwords(iters, (void *) h, hashdo);
}

LL *listwords(void) {
    LL *l = list_create();
    return (LL *) abstractwords(1, (void *) l, listdo);
}

LL *getwords(void) {
    return listwords();
}
