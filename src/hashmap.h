#ifndef _lcthw_hashmap_h
#define _lcthw_hashmap_h

#include "darray.h"
#include <stdint.h>

#define DEFAULT_NUMBER_OF_BUCKETS 100

typedef int (*hashmap_compare)(void *a, void *b);
typedef uint32_t (*hashmap_hash)(void *key);

typedef struct hashmap_t {
    darray_t *buckets;
    hashmap_compare compare;
    hashmap_hash hash;
} hashmap_t;

typedef struct hashmap_node_t {
    void *key;
    void *data;
    uint32_t hash;
} hashmap_node_t;

typedef int (*hashmap_traverse_cb)(hashmap_node_t *node);

hashmap_t *hashmap_create(hashmap_compare compare, hashmap_hash);
void hashmap_destroy(hashmap_t *map);

int hashmap_set(hashmap_t *map, void *key, void *data);
void *hashmap_get(hashmap_t *map, void *key);

int hashmap_traverse(hashmap_t *map, hashmap_traverse_cb traverse_cb);

void *hashmap_delete(hashmap_t *map, void *key);

#endif
