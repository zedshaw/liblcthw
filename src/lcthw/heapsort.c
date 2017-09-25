#include <stdlib.h>
#include <lcthw/dbg.h>

#include <lcthw/heapsort.h>

/**
 * Sorts an array `array[1..nmem]` of size `len` into ascending order according
 * to a compare function.
 *
 * This algorithm is written in the spirit of the heapsort as described in the second edition of
 * "Numberical Recipes in C" subtitled "The art of scientific computing" by William H. Press,
 * William T. Vetterling, Saul A. Teukolsky, Brian P. Flannery, rewritten to cater for a generic
 * array and a generic compare function.
 *
 * I have based the code on the ideas expressed in the book of Numerical Recipes.
 *
 */
int heapsort(void *array, size_t nmem, size_t size, compare_t compare)
{
    /** index into the array */
    size_t array_index;
    /** length of the sub array (the part that is not sorted yet). */
    size_t subarray_len;

    size_t sub_index;
    /** The index into the array and subarray for the current value of temp. */
    size_t current;
    /** The temp value that stores the element being evaluated.
     *
     *  Resist any temptation to use alloca, because we do not know how large size is.
     *  As a consequence we do need to free temp.
     * */
    char *temp = calloc(1, size);

    check(temp != NULL, "Allocate temp failed.");
    check(compare != NULL, "Comparing function should not be NULL.");
    check(array != NULL, "Array to be sorted should not be NULL.");

    /* check nmem: if zero or 1, return immediately (no sort necessary) */
    if (nmem < 2)
    {
        if (temp) free(temp);
        return 0;
    }

    /* Heapsort algorithm to be implemented */
    (void)(array_index);
    (void)(subarray_len);
    (void)(sub_index);
    (void)(current);
    /* Above: temporary until algorithm is implemented */

    if (temp) free(temp);
    return 0;
error:
    if (temp) free(temp);
    return -1;
}
