#include "hash.h"

#include "badhash.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Hashtable *hash_create(void) {
    Hashtable *hashtable = (Hashtable *) malloc(sizeof(Hashtable));
    if (!hashtable) {
        return NULL;
    } //hashtable is null
    for (int i = 0; i < BUCKETS; i++) {
        hashtable->bucket[i] = list_create(); //initialize each bucket to an empty list
    }
    return hashtable;
}

bool hash_put(Hashtable *hashtable, char *key, int val) {
    if (!hashtable || !key) {
        return NULL;
    } //ensure both hashtable and key point somewhere

    size_t bucket_to_put = hash(key) % BUCKETS; // finds bucket based on hash function
    //create item to put
    item to_put;
    to_put.id = val;
    strcpy(to_put.key, key);
    // attempt to add an item to the list at selected bucket
    return list_add(hashtable->bucket[bucket_to_put], &to_put);
}

int *hash_get(Hashtable *hashtable, char *key) {
    if (!hashtable || !key) {
        return NULL;
    } //ensure both hashtable and key point somewhere
    size_t bucket_to_get = hash(key) % BUCKETS; // finds bucket based on hash function

    item temp;
    strcpy(temp.key, key); //set of ket of item to find to key for hash to find
        //run list find to find item
    item *get_item = list_find(hashtable->bucket[bucket_to_get], cmp, &temp);
    //return ptr to value of the iteam if get_item != NULL otherwise return NULL
    return get_item ? &(get_item->id) : NULL;
}

void hash_destroy(Hashtable **hashtable) {
    for (int i = 0; i < BUCKETS; i++) {
        list_destroy((&(*hashtable)->bucket[i])); //iterate through buckets destroying every list
    }

    free(*hashtable); //free memory of hash table
    *hashtable = NULL; //set ptr to null
    return;
}
