#ifndef _lcthw_Queue_h
#define _lcthw_Queue_h

#include <lcthw/list.h>

typedef List Queue;

#define Queue_create List_create
#define Queue_destroy List_destroy
#define Queue_clear List_clear
#define Queue_clear_destroy List_clear_destroy

#define Queue_send List_unshift
#define Queue_recv List_pop

#define Queue_count List_count
#define Queue_peek List_last

#define QUEUE_FOREACH(Q, V) LIST_FOREACH(Q, last, prev, V)

#endif
