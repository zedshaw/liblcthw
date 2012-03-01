#undef NDEBUG
#include "hashmap.h"
#include "dbg.h"
#include <bstrlib.h>
#include <stdint.h>
#include <radixmap.h>

static int default_compare(void *a, void *b)
{
    return bstrcmp((bstring)a, (bstring)b);
}

/** 
 * Simple Bob Jenkin's hash algorithm taken from the
 * wikipedia description.
 */
static uint32_t default_hash(void *a)
{
    size_t len = blength((bstring)a);
    char *key = bdata((bstring)a);
    uint32_t hash = 0;
    uint32_t i = 0;

    for(hash = i = 0; i < len; ++i)
    {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}


hashmap_t *hashmap_create(hashmap_compare compare, hashmap_hash hash)
{
    hashmap_t *map = calloc(1, sizeof(hashmap_t));
    check_mem(map);

    map->compare = compare == NULL ? default_compare : compare;
    map->hash = hash == NULL ? default_hash : hash;
    map->buckets = darray_create(sizeof(darray_t *), DEFAULT_NUMBER_OF_BUCKETS);
    map->buckets->end = map->buckets->max; // fake out expanding it
    check_mem(map->buckets);

    return map;

error:
    if(map) {
        hashmap_destroy(map);
    }

    return NULL;
}


void hashmap_destroy(hashmap_t *map)
{
    int i = 0;
    int j = 0;

    // TODO: pack up this double-for-loop pattern into a macro somehow
    if(map) {
        if(map->buckets) {
            for(i = 0; i < darray_count(map->buckets); i++) {
                darray_t *bucket = darray_get(map->buckets, i);
                if(bucket) {
                    for(j = 0; j < darray_count(bucket); j++) {
                        free(darray_get(bucket, j));
                    }
                    darray_destroy(bucket);
                }
            }
            darray_destroy(map->buckets);
        }

        free(map);
    }
}

static inline hashmap_node_t *hashmap_node_create(int hash, void *key, void *data)
{
    hashmap_node_t *node = calloc(1, sizeof(hashmap_node_t));
    check_mem(node);

    node->key = key;
    node->data = data;
    node->hash = hash;

    return node;
error:
    return NULL;
}



int hashmap_set(hashmap_t *map, void *key, void *data)
{
    uint32_t hash = map->hash(key);
    int bucket_n = hash % DEFAULT_NUMBER_OF_BUCKETS;
    check(bucket_n >= 0, "Invalid bucket found: %d", bucket_n);

    darray_t *bucket = darray_get(map->buckets, bucket_n);

    if(!bucket) {
        // new bucket, set it up
        bucket = darray_create(sizeof(void *), DEFAULT_NUMBER_OF_BUCKETS);
        check_mem(bucket);
        darray_set(map->buckets, bucket_n, bucket);
    }

    hashmap_node_t *node = hashmap_node_create(hash, key, data);
    check_mem(node);

    // TODO: do sorting on the darray to make finding faster by the hash+key
    darray_push(bucket, node);

    return 0;

error:
    return -1;
}

void *hashmap_get(hashmap_t *map, void *key)
{
    int i = 0;
    uint32_t hash = map->hash(key);
    int bucket_n = hash % DEFAULT_NUMBER_OF_BUCKETS;
    check(bucket_n >= 0, "Invalid bucket found: %d", bucket_n);

    darray_t *bucket = darray_get(map->buckets, bucket_n);
    if(!bucket) return NULL;

    for(i = 0; i < darray_end(bucket); i++) {
        hashmap_node_t *node = darray_get(bucket, i);
        if(node->hash == hash && map->compare(node->key, key) == 0) {
            return node->data;
        }
    }

error: // fallthrough
    return NULL;
}


int hashmap_traverse(hashmap_t *map, hashmap_traverse_cb traverse_cb) {
    int i = 0;
    int j = 0;
    int rc = 0;

    for(i = 0; i < darray_count(map->buckets); i++) {
        darray_t *bucket = darray_get(map->buckets, i);
        if(bucket) {
            for(j = 0; j < darray_count(bucket); j++) {
                hashmap_node_t *node = darray_get(bucket, j);
                rc = traverse_cb(node);
                if(rc != 0) return rc;
            }
        }
    }

    return 0;
}
