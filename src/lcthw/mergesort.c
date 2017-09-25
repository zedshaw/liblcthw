#include <stdlib.h>
#include <lcthw/dbg.h>

#include <lcthw/heapsort.h>

/**
 * Sorts an array `array[1..nmem]` of size `len` into ascending order according
 * to a compare function.
 *
 * Not implemented yet.
 *
 */
int mergesort(void *array, size_t nmem, size_t size, compare_t compare)
{
    check(array != NULL, "Input array cannot be NULL");
    check(compare != NULL, "Compare function cannot be NULL");

    (void)(nmem);
    (void)(size);

    return 0;
error:
    return -1;
}
