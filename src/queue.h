#ifndef _lcthw_queue_h
#define _lcthw_queue_h

#include <list.h>

typedef list_t queue_t;

#define queue_create list_create
#define queue_destroy list_destroy
#define queue_clear list_clear
#define queue_clear_destroy list_clear_destroy

#define queue_send list_shift
#define queue_recv list_pop

#define queue_count list_count
#define queue_peek list_last

#define QUEUE_FOREACH(Q, V) LIST_FOREACH(Q, last, prev, V)

#endif
