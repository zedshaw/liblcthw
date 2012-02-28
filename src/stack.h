#ifndef _lcthw_stack_h
#define _lcthw_stack_h

#include <list.h>

typedef list_t Stack_t;

#define stack_create list_create
#define stack_destroy list_destroy
#define stack_clear list_clear
#define stack_clear_destroy list_clear_destroy

#define stack_push list_push
#define stack_pop list_pop
#define stack_peek list_last

#define stack_count list_count

#define STACK_FOREACH(S, V) LIST_FOREACH(S, last, prev, V)

#endif
