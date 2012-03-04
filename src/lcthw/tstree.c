#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <lcthw/dbg.h>
#include <lcthw/tstree.h>

typedef struct TSTreeCollect {
    DArray *values;
    TSTree_collect_test_cb tester;
    const char *key;
    size_t len;
} TSTreeCollect;

enum {
    INITIAL_DARRAY_SIZE = 20
};

static void TSTree_collect_build(void *value, TSTreeCollect *results)
{
    if(!results->tester || results->tester(value, results->key, results->len)) {
        DArray_push(results->values, value);
    }
}

DArray *TSTree_collect(TSTree *root, const char *s, size_t len, TSTree_collect_test_cb tester)
{
    TSTreeCollect results = {.values = NULL, .tester = tester, .key = s, .len = len};
    TSTree *p = root;
    TSTree *last = p;
    size_t i = 0;
    results.values = DArray_create(sizeof(void *), INITIAL_DARRAY_SIZE);

    // first we get to where we match the prefix
    while(i < len && p) {
        if (s[i] < p->splitchar) {
            p = p->low; 
        } else if (s[i] == p->splitchar) {
            i++;
            if(i < len) {
                if(p->value) last = p;
                p = p->equal; 
            }
        } else {
            p = p->high; 
        }
    }

    if((last && results.tester) || p) {
        // we found node matching this prefix, so traverse and collect
        TSTree_traverse(p == NULL ? last : p, (TSTree_traverse_cb)TSTree_collect_build, &results);
    }

    return results.values;
}

void *TSTree_search_suffix(TSTree *root, const char *s, size_t len)
{
    if(len == 0) return NULL;

    TSTree *p = root;
    TSTree *last = NULL;
    int i = len-1;

    while(i >= 0 && p) {
        if (s[i] < p->splitchar) {
            p = p->low; 
        } else if (s[i] == p->splitchar) {
            i--;
            if(i >= 0) {
                if(p->value) last = p;
                p = p->equal;
            }
        } else {
            p = p->high; 
        }
    }

    p = p ? p : last;

    while(p && !p->value) {
        p = p->equal;
    }

    return p ? p->value : NULL;
}

void *TSTree_search_prefix(TSTree *root, const char *s, size_t len)
{
    if(len == 0) return NULL;

    TSTree *p = root;
    TSTree *last = NULL;
    size_t i = 0;

    while(i < len && p) {
        if (s[i] < p->splitchar) {
            p = p->low; 
        } else if (s[i] == p->splitchar) {
            i++;
            if(i < len) {
                if(p->value) last = p;
                p = p->equal;
            }
        } else {
            p = p->high; 
        }
    }

    p = p ? p : last;

    // traverse until we find the first value in the chain of splitchars
    while(p && !p->value) {
        p = p->equal;
    }

    return p ? p->value : NULL;
}

void *TSTree_search(TSTree *root, const char *s, size_t len)
{
    TSTree *p = root;
    size_t i = 0;

    while(i < len && p) {

        if (s[i] < p->splitchar) {
            p = p->low; 
        } else if (s[i] == p->splitchar) {
            i++;
            if(i < len) p = p->equal; 
        } else {
            p = p->high; 
        }
    }

    if(p) {
        return p->value;
    } else {
        return NULL; 
    }
}


static inline TSTree *TSTree_insert_base(TSTree *root, TSTree *p, const char *s, size_t len, void *value)
{
    if (p == NULL) { 
        p = (TSTree *) calloc(1, sizeof(TSTree));

        if(root == NULL) {
            root = p;
        }

        p->splitchar = *s; 
    }

    if (*s < p->splitchar) {
        p->low = TSTree_insert_base(root, p->low, s, len, value); 
    } else if (*s == p->splitchar) { 
        if (len > 1) {
            // not done yet, keep going but one less
            p->equal = TSTree_insert_base(root, p->equal, s+1, len - 1, value);
        } else {
            assert(p->value == NULL && "Duplicate insert into tst.");
            p->value = value;
        }
    } else {
        p->high = TSTree_insert_base(root, p->high, s, len, value);
    }

    return p; 
}

TSTree *TSTree_insert(TSTree *p, const char *s, size_t len, void *value)
{
    return TSTree_insert_base(p, p, s, len, value);
}


void TSTree_traverse(TSTree *p, TSTree_traverse_cb cb, void *data)
{
    if (!p) return;

    if(p->low) TSTree_traverse(p->low, cb, data);

    if (p->equal) {
        TSTree_traverse(p->equal, cb, data); 
    }

    if(p->high) TSTree_traverse(p->high, cb, data); 

    if(p->value) cb(p->value, data);
}

void TSTree_destroy(TSTree *p)
{
    if(p->low) TSTree_destroy(p->low);

    if (p->equal) {
        TSTree_destroy(p->equal);
    }

    if(p->high) TSTree_destroy(p->high);

    if(p) free(p);
}
