#ifndef _lcthw_TSTree_h
#define _lctwh_TSTree_h

#include <stdlib.h>
#include <lcthw/darray.h>

typedef struct TSTree { 
    char splitchar; 
    struct TSTree *low;
    struct TSTree *equal;
    struct TSTree *high; 
    void *value;
} TSTree; 


typedef void (*TSTree_traverse_cb)(void *value, void *data);
typedef int (*TSTree_collect_test_cb)(void *value, const char *path, size_t len);


// won't work unless you reverse before insert, useful though
// for looking up things from last to first char, as in hostnames
void *TSTree_search_suffix(TSTree *root, const char *s, size_t len);

void *TSTree_search(TSTree *root, const char *s, size_t len);

void *TSTree_search_prefix(TSTree *root, const char *s, size_t len);

TSTree *TSTree_insert(TSTree *p, const char *s, size_t len, void *value);

void TSTree_traverse(TSTree *p, TSTree_traverse_cb cb, void *data);

DArray *TSTree_collect(TSTree *root, const char *s, size_t len, TSTree_collect_test_cb tester);

void TSTree_destroy(TSTree *root);

#endif
