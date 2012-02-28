#ifndef lcthw_list_h
#define lcthw_list_h

#include <stdlib.h>

struct list_node_t;

typedef struct list_node_t {
    struct list_node_t *next;
    struct list_node_t *prev;
    void *value;
} list_node_t;

typedef struct list_t {
    int count;
    list_node_t *first;
    list_node_t *last;
} list_t;

list_t *list_create();
void list_destroy(list_t *list);
void list_clear(list_t *list);
void list_clear_destroy(list_t *list);

#define list_count(A) ((A)->count)
#define list_first(A) ((A)->first != NULL ? (A)->first->value : NULL)
#define list_last(A) ((A)->last != NULL ? (A)->last->value : NULL)

typedef int (*list_traverse_cb)(list_node_t *node, void *data);

int list_traverse(list_t *list, list_traverse_cb cb, void *data);

void list_push(list_t *list, void *value);
void *list_pop(list_t *list);

void list_shift(list_t *list, void *value);
void *list_unshift(list_t *list);

void *list_remove(list_t *list, list_node_t *node);
void *list_search(list_t *list, list_traverse_cb cb, void *data);

#define LIST_FOREACH(L, S, M, V) list_node_t *_node = NULL;\
    list_node_t *V = NULL;\
    for(V = _node = L->S; _node != NULL; V = _node = _node->M)

#endif
