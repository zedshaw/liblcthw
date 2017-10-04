#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <lcthw/dbg.h>

#include <lcthw/mergesort.h>

typedef struct
{

} part_list_t;
typedef struct
{
} part_info_t;

/* prototypes */
int merge_move_mem(size_t member_size, void *to, void *from);
int merge(void *array, size_t nmem, size_t member_size, char *scratch,
          compare_t compare, size_t left, size_t llen, size_t right, size_t rlen,
          void (*print)(void * const array, const size_t nmem, const void *temp));

/**
 * Moves memory of a specified size from one location to another and
 * initializes the from location.
 *
 * @param member_size size_t the amount of memory being moved
 * @param to void * pointer to the destination
 * @param from void * pointer to the origin
 * @return zero if all went well, -1 if an error occurred.
 *
 * Remark: this function has no way of checking the validity of the
 * memory addresses. This is the callers responsibility.
 */
int merge_move_mem(size_t member_size, void *to, void *from)
{
    check(member_size != 0, "Member size can not be zero.");
    check(to != NULL, "Cannot copy memory to NULL.");
    check(from != NULL, "Cannot copy memory from NULL.");

    char *ret = memcpy(to, from, member_size);
    check(ret != NULL, "Memory copy in hiring phase failed");

    ret = memset(from, '\0', member_size);
    check(ret != NULL, "Memory init of vacancy position failed");

    return 0;
error:
    return -1;
}

/**
 * Sorts an array `array[1..nmem]` of member_size `len` into ascending order according
 * to a compare function.
 *
 * We divide the array into nmem / n equal parts of n members where n doubles each run.
 *
 * The iteration works bottom up from partitions of 1 member doubling its length
 * each run. In the last run only one whole partition of n members remains and
 * a tail where nmem/2 < n <= nmem. If n == nmem then no more sorting will be done
 * as the tail has length zero.
 *
 * @param array void * The pointer to the array.
 * @param nmem size_t the number of members
 * @param member_size size_t the size of one member of the array
 * @param compare compare_t function pointer to compare function
 * @return zero if all is well, -1 if something went wrong.
 *
 */
int mergesort(void *array, size_t nmem, size_t member_size, compare_t compare)
{
    return __mergesort(array, nmem, member_size, compare, NULL);
}

int __mergesort(void *array, size_t nmem, size_t member_size, compare_t compare,
                void (*print)(void * const array, const size_t nmem, const void *temp)
               )
{
    char *scratch = NULL;

    check(array != NULL, "Input array cannot be NULL");
    check(compare != NULL, "Compare function cannot be NULL");

    /** Create a scratch area of equal size to the array */
    scratch = malloc(nmem * member_size);
    errno = ENOMEM;
    check(scratch != NULL, "Error allocating scratch");

    size_t nparts;			/** The number of full partitions. */
    size_t npaired_parts;	/** The number of parts to be compared of equal size */
    _Bool has_single_part;		/** The last odd part if present */
    size_t tail_len;		/** The length of the last partition */
    /*
     * divide the array into nmem / n equal parts of n members where n doubles each run.
     */
    for(size_t part_len=1; part_len<nmem; part_len<<=1)
    {
        nparts = nmem / part_len;				/* Integer division: truncates */
        npaired_parts = nparts / 2;
        has_single_part = nparts % 2;
        tail_len = nmem % part_len;

        /* merge the sets of partitions of equal size nmem / n */
        for (size_t i = 0; i < npaired_parts; i++)
        {
            size_t left_offset = 2*i*part_len;
            size_t right_offset = left_offset + part_len;
            int r = merge(array, nmem, member_size, scratch, compare,
                          left_offset, part_len, right_offset, part_len, print);
            check(r == 0, "merge of 2 equal parts failed.");
        }

        /* merge last 2 partitions if there are 2 partitions */
        if (has_single_part == true)
        {
            /* Remark: the very last run will always have a single partition,
             * with or without a tail */

            /* Merge last 2 partitions: if tail is empty nothing happens. */
            size_t start_single_part = 2 * npaired_parts * part_len;
            size_t start_tail = start_single_part + part_len;
            int r = merge(array, nmem, member_size, scratch, compare,
                          start_single_part, part_len, start_tail, tail_len, print);
            check(r == 0, "Merge single part and tail failed.");
        }
        if (print) print(array, nmem, 0);
        if (print) print(scratch, nmem, 0);
    }

    /* Free the allocated array */
    if (scratch) free(scratch);
    return 0;
error:
    /* Free the allocated array */
    if (scratch) free(scratch);
    return -1;
}
/**
 * merges two sorted segments of an array keeping the elements in order and
 * preserving the order between equal keys.
 *
 * @param array void * The pointer to the array.
 * @param nmem size_t the number of members
 * @param member_size size_t the size of one member of the array
 * @param compare compare_t function pointer to compare function
 * @param left size_t the offset of the left partition
 * @param llen size_t the length of the left partition
 * @param right size_t the offset of the right partition
 * @param rlen size_t the length of the right partition
 * if defined NDEBUG:
 *    @param print void (*)(void * const array, const size_t nmem, const void *temp)
 *					The print function provided by caller: is allowed to be NULL.
 * @return zero if all is well, -1 if something went wrong.
 */
int merge(void *array, size_t nmem, size_t member_size, char *scratch, compare_t compare,
          size_t left, size_t llen, size_t right, size_t rlen,
          void (*print)(void * const array, const size_t nmem, const void *temp))
{
    /* Check the input */
    errno = EINVAL;
    check(array != NULL, "Cannot merge a NULL array.");
    errno = EINVAL;
    check(member_size != 0, "member_size cannot be zero.");
    errno = EINVAL;
    check(scratch != NULL, "Scratch area should not be null.");
    errno = EINVAL;
    check(compare != NULL, "Compare should not be NULL");
    errno = EINVAL;
    check(left+llen <= nmem,
          "Left partition should not extend beyond array: (left, llen, nmem) = (%lu, %lu, %lu).",
          left, llen, nmem);
    errno = EINVAL;
    check(right+rlen <= nmem,
          "Right partition should not extend beyond array: (right, rlen, nmem) = (%lu, %lu, %lu).",
          right, rlen, nmem);
    errno = EINVAL;
    check(left+llen == right, "Partitions should be consecutive");

    /* Guard for empty arrays and arrays with 1 element */
    if (nmem < 2) return 0;

    /* Guard for empty partitions */
    if (llen == 0) return 0;
    if (rlen == 0) return 0;

    /* Now we start the merge */

    /* calculate the total length of both partitions together */
    size_t tot_len = rlen+llen;
    /* Copy the subsequent partition members to the scratch array in one go */
    void *scratch_addr = scratch + left * member_size;
    void *array_addr = array + left * member_size;
    size_t chunk_len = tot_len * member_size;
    memcpy(scratch_addr, array_addr, chunk_len);
    memset(array_addr, '\0', chunk_len);

    /* Merge both array parts from the allocated array to the original array */
    size_t i = 0;
    size_t j = llen;
    size_t merge_pt = 0;
    while (merge_pt < tot_len)
    {
        check(i + j - llen  == merge_pt,
              "Merge point somehow degraded: (i, j, merge_pt) = (%zd, %zd, %zd)."
              "\n(left, llen) = (%zd, %zd)",
              i, j, merge_pt, left, llen);
        if (print)
        {
            printf("array  : ");
            print(array, nmem, NULL);
            printf("scratch: ");
            print(scratch, nmem, NULL);
        }
        if (i < llen && j < tot_len)
        {
            /* compare and move smallest to destination */
            void *left_addr = scratch + (i + left) * member_size;
            void *right_addr = scratch + (j + left) * member_size;
            if (compare(left_addr, right_addr) <= 0)
            {
                int r = merge_move_mem(member_size, array+(merge_pt + left)*member_size,
                                       scratch+(i+left)*member_size);
                check(r == 0, "merge_move_mem of memory chunk left partition failed.");
                i++;
                merge_pt++;
            }
            else
            {
                int r = merge_move_mem(member_size, array+(merge_pt+left)*member_size,
                                       scratch+(j+left)*member_size);
                check(r == 0, "merge_move_mem of memory chunk right partition failed.");
                j++;
                merge_pt++;
            }
        }
        else if (i < llen)
        {
            /* move left partition to destination */
            int r = merge_move_mem(member_size, array+(merge_pt+left)*member_size,
                                   scratch+(i+left)*member_size);
            check(r == 0, "merge_move_mem of memory chunk left partition failed.");
            i++;
            merge_pt++;
        }
        else if (j < tot_len)
        {
            /* move right partition to destination */
            int r = merge_move_mem(member_size, array+(merge_pt+left)*member_size,
                                   scratch+(j+left)*member_size);
            check(r == 0, "merge_move_mem of memory chunk right partition failed.");
            j++;
            merge_pt++;
        }

    }

    return 0;
error:
    return -1;
}
