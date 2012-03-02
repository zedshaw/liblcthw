#ifndef _lcthw_btree_h
#define _lcthw_btree_h


typedef int (*btree_compare)(void *a, void *b);

typedef struct btree_node_t {
    void *key;
    void *data;

    struct btree_node_t *left;
    struct btree_node_t *right;
    struct btree_node_t *parent;
} btree_node_t;

typedef struct btree_t {
    int count;
    btree_compare compare;
    btree_node_t *root;
} btree_t;

typedef int (*btree_traverse_cb)(btree_node_t *node);

btree_t *btree_create(btree_compare compare);
void btree_destroy(btree_t *map);

int btree_set(btree_t *map, void *key, void *data);
void *btree_get(btree_t *map, void *key);

int btree_traverse(btree_t *map, btree_traverse_cb traverse_cb);

void *btree_delete(btree_t *map, void *key);

#endif
