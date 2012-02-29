#ifndef _lcthw_hashmap_h
#define _lcthw_hashmap_h

#include "darray.h"

typedef struct hashmap_t {
    darray_t buckets;
    int count;
} hashmap_t;

typedef int (*hashmap_compare)(void *a, void *b);

hashmap_t *hashmap_create(hashmap_compare cb);
void hashmap_destroy(hashmap_t *map);
void hashmap_clear(hashmap_t *map);
void hashmap_clear_destroy(hashmap_t *map);

int hashmap_set(hashmap_t *map, void *key, void *data);
void *hashmap_get(hashmap_t *map, void *key);

#define HASHMAP_FOREACH(H, V) 
#endif
