#undef NDEBUG
#include "btree.h"
#include <stdlib.h>
#include "dbg.h"
#include <bstrlib.h>

static int default_compare(void *a, void *b)
{
    return bstrcmp((bstring)a, (bstring)b);
}


btree_t *btree_create(btree_compare compare)
{
    btree_t *map = calloc(1, sizeof(btree_t));
    check_mem(map);

    map->compare = compare == NULL ? default_compare : compare;

    return map;

error:
    if(map) {
        btree_destroy(map);
    }
    return NULL;
}

void btree_destroy(btree_t *map)
{
    if(map) {
        // TOOD: complete this
        free(map);
    }
}


static inline btree_node_t *btree_node_create(void *key, void *data)
{
    btree_node_t *node = calloc(1, sizeof(btree_node_t));
    check_mem(node);

    node->key = key;
    node->data = data;
    return node;

error:
    return NULL;
}


static inline void btree_setnode(btree_t *map, btree_node_t *node, void *key, void *data)
{
    int cmp = map->compare(node->key, key);

    if(cmp <= 0) {
        if(node->left) {
            btree_setnode(map, node->left, key, data);
        } else {
            node->left = btree_node_create(key, data);
        }
    } else {
        if(node->right) {
            btree_setnode(map, node->right, key, data);
        } else {
            node->right = btree_node_create(key, data);
        }
    }
}


int btree_set(btree_t *map, void *key, void *data)
{
    if(map->root == NULL) {
        // first so just make it and get out
        map->root = btree_node_create(key, data);
        check_mem(map->root);
    } else {
        btree_setnode(map, map->root, key, data);
    }

    return 0;
error:
    return -1;
}

static inline btree_node_t *btree_getnode(btree_t *map, btree_node_t *node, void *key)
{
    int cmp = map->compare(node->key, key);

    if(cmp == 0) {
        return node;
    } else if(cmp < 0) {
        return btree_getnode(map, node->left, key);
    } else {
        return btree_getnode(map, node->right, key);
    }
}

void *btree_get(btree_t *map, void *key)
{
    if(map->root == NULL) {
        return NULL;
    } else {
        btree_node_t *node = btree_getnode(map, map->root, key);
        return node == NULL ? NULL : node->data;
    }
}


static inline int btree_traverse_nodes(btree_node_t *node, btree_traverse_cb traverse_cb)
{
    int rc = 0;

    // TODO: probably the totally wrong order on this
    if(node->left) {
        rc = btree_traverse_nodes(node->left, traverse_cb);
        if(rc != 0) return rc;
    }

    if(node->right) {
        rc = btree_traverse_nodes(node->right, traverse_cb);
        if(rc != 0) return rc;
    }

    return traverse_cb(node);
}

int btree_traverse(btree_t *map, btree_traverse_cb traverse_cb)
{
    if(map->root) {
        btree_traverse_nodes(map->root, traverse_cb);
    }
}

