#ifndef MERGESORT_H
#define MERGESORT_H

#include <stdlib.h>
#include <lcthw/sort.h>

/* prototypes */
int mergesort(void *array, size_t nmem, size_t size, compare_t compare);
int __mergesort(void *array, size_t nmem, size_t size, compare_t compare,
                void (*print)(void * const array, const size_t nmem, const void * temp)
               );

#endif
