#include "list.h"
#include "dbg.h"
#include <assert.h>


list_t *list_create()
{
    return calloc(1, sizeof(list_t));
}

void list_destroy(list_t *list)
{
    LIST_FOREACH(list, first, next, cur) {
        if(cur->prev) {
            free(cur->prev);
        }
    }

    free(list->last);
    free(list);
}


void list_clear(list_t *list)
{
    LIST_FOREACH(list, first, next, cur) {
        free(cur->value);
    }
}


void list_clear_destroy(list_t *list)
{
    list_clear(list);
    list_destroy(list);
}


void list_push(list_t *list, void *value)
{
    list_node_t *node = calloc(1, sizeof(list_node_t));
    check_mem(node);

    node->value = value;

    if(list->last == NULL) {
        list->first = node;
        list->last = node;
    } else {
        list->last->next = node;
        node->prev = list->last;
        list->last = node;
    }

    list->count++;

error:
    return;
}

void *list_pop(list_t *list)
{
    list_node_t *node = list->last;
    return node != NULL ? list_remove(list, node) : NULL;
}

void list_shift(list_t *list, void *value)
{
    list_node_t *node = calloc(1, sizeof(list_node_t));
    check_mem(node);

    node->value = value;

    if(list->first == NULL) {
        list->first = node;
        list->last = node;
    } else {
        node->next = list->first;
        list->first->prev = node;
        list->first = node;
    }

    list->count++;

error:
    return;
}

void *list_unshift(list_t *list)
{
    list_node_t *node = list->first;
    return node != NULL ? list_remove(list, node) : NULL;
}

void *list_remove(list_t *list, list_node_t *node)
{
    void *result = NULL;

    check(list->first && list->last, "List is empty.");
    check(node, "node can't be NULL");

    if(node == list->first && node == list->last) {
        list->first = NULL;
        list->last = NULL;
    } else if(node == list->first) {
        list->first = node->next;
    } else if (node == list->last) {
        list->last = node->prev;
    } else {
        list_node_t *after = node->next;
        list_node_t *before = node->prev;
        after->prev = before;
        before->next = after;
    }

    list->count--;
    result = node->value;
    free(node);

error:
    return result;
}


