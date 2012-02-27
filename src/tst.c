#include <stdlib.h>
#include "tst.h"
#include <stdio.h>
#include <assert.h>
#include <dbg.h>

typedef struct tst_collect_t {
    darray_t *values;
    tst_collect_test_cb tester;
    const char *key;
    size_t len;
} tst_collect_t;

enum {
    INITIAL_DARRAY_SIZE = 20
};

static void tst_collect_build(void *value, tst_collect_t *results)
{
    if(!results->tester || results->tester(value, results->key, results->len)) {
        darray_push(results->values, value);
    }
}

darray_t *tst_collect(tst_t *root, const char *s, size_t len, tst_collect_test_cb tester)
{
    tst_collect_t results = {.values = NULL, .tester = tester, .key = s, .len = len};
    tst_t *p = root;
    tst_t *last = p;
    size_t i = 0;
    results.values = darray_create(sizeof(void *), INITIAL_DARRAY_SIZE);

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
        tst_traverse(p == NULL ? last : p, (tst_traverse_cb)tst_collect_build, &results);
    }

    return results.values;
}

void *tst_search_suffix(tst_t *root, const char *s, size_t len)
{
    if(len == 0) return NULL;

    tst_t *p = root;
    tst_t *last = NULL;
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

void *tst_search_prefix(tst_t *root, const char *s, size_t len)
{
    if(len == 0) return NULL;

    tst_t *p = root;
    tst_t *last = NULL;
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

void *tst_search(tst_t *root, const char *s, size_t len)
{
    tst_t *p = root;
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


static inline tst_t *tst_insert_base(tst_t *root, tst_t *p, const char *s, size_t len, void *value)
{
    if (p == NULL) { 
        p = (tst_t *) calloc(1, sizeof(tst_t));

        if(root == NULL) {
            root = p;
        }

        p->splitchar = *s; 
    }

    if (*s < p->splitchar) {
        p->low = tst_insert_base(root, p->low, s, len, value); 
    } else if (*s == p->splitchar) { 
        if (len > 1) {
            // not done yet, keep going but one less
            p->equal = tst_insert_base(root, p->equal, s+1, len - 1, value);
        } else {
            assert(p->value == NULL && "Duplicate insert into tst.");
            p->value = value;
        }
    } else {
        p->high = tst_insert_base(root, p->high, s, len, value);
    }

    return p; 
}

tst_t *tst_insert(tst_t *p, const char *s, size_t len, void *value)
{
    return tst_insert_base(p, p, s, len, value);
}


void tst_traverse(tst_t *p, tst_traverse_cb cb, void *data)
{
    if (!p) return;

    if(p->low) tst_traverse(p->low, cb, data);

    if (p->equal) {
        tst_traverse(p->equal, cb, data); 
    }

    if(p->high) tst_traverse(p->high, cb, data); 

    if(p->value) cb(p->value, data);
}

void tst_destroy(tst_t *p)
{
    if(p->low) tst_destroy(p->low);

    if (p->equal) {
        tst_destroy(p->equal);
    }

    if(p->high) tst_destroy(p->high);

    if(p) free(p);
}
