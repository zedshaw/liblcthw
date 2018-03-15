#ifndef HEAPSORT_H
#define HEAPSORT_H

#include <stdlib.h>
#include <lcthw/sort.h>

/* prototypes */
int heapsort(void * const array, const size_t nmem, const size_t size, compare_t compare);
int __heapsort(void * const array, const size_t nmem, const size_t size, compare_t compare,
               void (*debug_print)(void * const array, const size_t size, const void *temp));

#endif
