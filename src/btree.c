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

static int btree_destroy_cb(btree_node_t *node)
{
    free(node);
    return 0;
}

void btree_destroy(btree_t *map)
{
    if(map) {
        btree_traverse(map, btree_destroy_cb);
        free(map);
    }
}


static inline btree_node_t *btree_node_create(btree_node_t *parent, void *key, void *data)
{
    btree_node_t *node = calloc(1, sizeof(btree_node_t));
    check_mem(node);

    node->key = key;
    node->data = data;
    node->parent = parent;
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
            node->left = btree_node_create(node, key, data);
        }
    } else {
        if(node->right) {
            btree_setnode(map, node->right, key, data);
        } else {
            node->right = btree_node_create(node, key, data);
        }
    }
}


int btree_set(btree_t *map, void *key, void *data)
{
    if(map->root == NULL) {
        // first so just make it and get out
        map->root = btree_node_create(NULL, key, data);
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
        if(node->left) {
            return btree_getnode(map, node->left, key);
        } else {
            return NULL;
        }
    } else {
        if(node->right) {
            return btree_getnode(map, node->right, key);
        } else {
            return NULL;
        }
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

    // TODO: probably the totally wrong order on this but works for destroy
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
        return btree_traverse_nodes(map->root, traverse_cb);
    }

    return 0;
}

static inline btree_node_t *btree_find_min(btree_node_t *node)
{
    while(node->left) {
        node = node->left;
    }

    return node;
}

static inline void btree_replace_node_in_parent(btree_t *map, btree_node_t *node, btree_node_t *new_value)
{
    if(node->parent) {
        if(node == node->parent->left) {
            node->parent->left = new_value;
        } else {
            node->parent->right = new_value;
        }
    } else {
        // this is the root so gotta change it
        map->root = new_value;
    }

    if(new_value) {
        new_value->parent = node->parent;
    }
}

static inline btree_node_t *btree_node_delete(btree_t *map, btree_node_t *node, void *key)
{
    int cmp = map->compare(node->key, key);

    if(cmp < 0) {
        return btree_node_delete(map, node->left, key);
    } else if(cmp > 0) {
        return btree_node_delete(map, node->right, key);
    } else {
        if(node->left && node->right) {
            btree_node_t *successor = btree_find_min(node);
            btree_replace_node_in_parent(map, node, successor->right);
        } if(node->left) {
            btree_replace_node_in_parent(map, node, node->left);
        } else if(node->right) {
            btree_replace_node_in_parent(map, node, node->right);
        } else {
            btree_replace_node_in_parent(map, node, NULL);
        }

        return node;
    }
}

void *btree_delete(btree_t *map, void *key)
{
    btree_node_t *node = btree_node_delete(map, map->root, key);
    void *data = node->data;
    free(node);

    return data;
}

