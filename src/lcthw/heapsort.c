/* Copyright Guus Bonnema, 2017.
 * User license according to the accompanied LICENSE file
 * or (at your choice) the GPL version 3 or later.
 *
 * For questions about the heapsort implementation you can contact me through
 * github at user gbonnema.
 */

/**
 * heapsort sorts an array `array[1..nmem]` of size `member_size` into ascending order according
 * to a compare function that the caller provides.
 *
 * This algorithm is written in the spirit of the heapsort as described in the second edition of
 * "Numerical Recipes in C" subtitled "The art of scientific computing" by William H. Press,
 * William T. Vetterling, Saul A. Teukolsky, Brian P. Flannery, written to cater for a generic
 * array and a generic compare function.
 *
 * ** General construction
 *
 *  A heap is a binary tree filled from top to bottom and from left to right.
 *  For each item k (supervisor) the items 2k and 2k+1 are the children (subordinates)
 *  often called parents and children, or supervisors and workers.
 *
 *  In the analogy of Numerical recipes, the sort consists of phases:
 *
 *      1. a company being created, starting with n/2 workers and hiring supervisors.
 *         Each supervisor has 2 workers / supervisors to look after.
 *      2. Once created, being a heap, in each triangle of supervisor and subordinates
 *         the supervisor has the biggest value. I.e. the root of each subtree is the largest
 *         of both its children.
 *
 *         Sorting consists of retiring the big boss (the root) into the last available
 *         position, displacing the last position to temp.
 *         This is followed by promotions until the temp has found its rightful place
 *         in the tree. The heap shrinks by one (it lost one employee, the former boss).
 *
 *         After the last employee retires the array is sorted from smallest to largest.
 *         Remark that the order depends on how the compare function works. If the compare
 *         works reversed the array will be sorted from largest to smallest.
 *
 *  General remark: the word "position" is both a link to the boss-worker analogy and
 *  an indication of counting: it counts from 1. Under the covers when doing a locate
 *  in the array, this is translated to an offset ( = position - 1). The rationale is
 *  that the mathematical rules for heap only work if you count from 1, not if you count
 *  from zero. the relationship boss = k and subordinates are 2k and 2k+1 only work if
 *  k is 1-based, not if its zero-based.
 *
 *  **************************************
 *  ** Heap construction (hiring phase) **
 *  **************************************
 *
 *  The position of the vacancy position will be decremented from its initial value
 *  down to 1 during the "hiring" (heap creation) phase.
 *  Once vacancy position reaches 1, the hiring phase finishes and "retirement and
 *  promotion" (sorting phase) starts. Every time a boss retires, it takes the space of
 *  the last lowly worker who is stored in temp. Then the promotion run begins until we
 *  find the spot for the temp.
 *
 *  Both phases use the same promotion procedure to promote until we find the spot
 *  for the temp (which is either a new hire or the lowly worker that was displaced by
 *  the boss).
 *
 *  In the "hiring" phase, the vacancies will progress backwards toward 1, after which
 *  the heap is complete.
 *  After the hiring phase we have one heap consisting of supervisors in the
 *  top half and workers in the bottom half. The Chief Officer in position 1 is the boss
 *  and the largest item and in the root of the tree.
 *
 *  The invariant of the hiring phase is:
 *
 *  1. The bottom elements consist of consequtive heaps.
 *
 *  During the hiring phase the heaps part expands to include the whole array into one heap.
 *
 *  **********************************************
 *  ** Sorting phase (retirement and promotion) **
 *  **********************************************
 *
 *  In the "retirement and promotion" phase, the heap_size will shrink from its
 *  initial value down to 1. After this the array is sorted.
 *
 *  Once the heap is formed, sorting is easy. The first item is alway the largest. So
 *  the boss retires to the last place in the array, the displaced worker becomes a temp,
 *  and the promotion ritual starts and finished when the temps finds its rightful place
 *  in the tree. The heap is now one element smaller and the largest is at the end. Now
 *  retirement starts again and the new boss retires to the last place in the heap (right
 *  in front of the previous boss).
 *
 *  At the end the heap the last employee retires, the heap becomes empty and the
 *  whole array is sorted. Remark that the ordering of double values is not guaranteed
 *  to be preserved. This could be relevant during sort of a struct where multiple
 *  key values could occur with different properties (other than key).
 *
 *  The invariants in the sorting phase are
 *
 *      1. The heap part remains a heap before and after the phase.
 *      2. The elements after the heap are in sorted order before and after the phase.
 *
 *  When the retirement phase starts, the heap is the full array and the sorted part is empty.
 *  When the retirement phase is finished the heap is empty and the array is sorted.
 *
 *  The implementation of the compare function determines the sort order.
 *
 *  ** Remark **
 *
 *  To debug one calls __heapsort with an extra function taking the array, its number of members
 *  and the current temp value as a parameter. If used, and the function is non NULL,
 *  it will be called. What it could do is print or examine the array and temp value.
 *  This is only meant for debug and NDEBUG should not be defined for this to work.
 *
 */
#include <stdlib.h>
#include <lcthw/dbg.h>
#include <lcthw/heapsort.h>

typedef enum PHASE
{
    HIRING_PHASE, RETIREMENT_PHASE, SORT_FINISHED_PHASE, ERROR_PHASE
} phase_t;

/* prototypes */
_Bool has_right_child(size_t parent, size_t array_len);
size_t left_child(size_t parent);
size_t right_child(size_t parent);
size_t offset(size_t position);
void *locate(void *array, size_t member_size, size_t nmem, size_t position);
size_t compare_children(void * array, size_t member_size, size_t nmem,
                        size_t parent, compare_t compare);
phase_t new_hire(void * array, size_t member_size, size_t nmem, size_t vacancy_pos, void *temp);
phase_t retire_boss(void *array, size_t member_size,
                    size_t heap_size, void *temp);
int heapify(void *array, size_t member_size, size_t heap_size,
            size_t promotion_pos, void *temp, compare_t compare);
/* end prototypes */


_Bool has_right_child(size_t parent, size_t array_len)
{
    return right_child(parent) <= array_len;
}

_Bool has_left_child(size_t parent, size_t array_len)
{
    return left_child(parent) <= array_len;
}

size_t left_child(size_t parent)
{
    size_t left_child = 2 * parent;
    return left_child;
}

size_t right_child(size_t parent)
{
    size_t right_child = 2 * parent + 1;
    return right_child;
}

/**
 * returns the offset to a specified position.
 *
 * While most functions in this file work from position (1-based)
 * this is the only function that turns a position to an offset.
 *
 * @param position size_t the 1-based index to an item in an array.
 * @return size_t the offset of an item in an array (0-based).
 */
size_t offset(size_t position)
{
    return position - 1;
}

/**
 * Returns the address of a position in an array on basis of the
 * 1-based position and the member_size of each array member.
 *
 * @param array void * pointer to the array
 * @param member_size size_t the size of one member in the array
 * @param position size_t the 1-based position of an item in an array
 * @param nmem size_t the number of members in the array
 * @return the address of the specified position in an array.
 */
void * locate(void * array, size_t member_size, size_t nmem, size_t position)
{
    check(position <= nmem, "Locate out of bounds: position = %lu, nmem = %lu.",
          position, nmem);
    return array + offset(position)*member_size;
error:
    return NULL;
}
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
int heap_move_mem(size_t member_size, void *to, void *from)
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

size_t compare_children(void * array, size_t member_size, size_t nmem,
                        size_t parent, compare_t compare)
{
    check(array != NULL, "Array can not be NULL.");
    check(member_size > 0, "Member size can not be zero.");
    check(parent != 0, "Parent can not be zero (is 1-based).");
    check(parent <= nmem, "Compare_children out of bounds: parent %lu, nmem %lu.",
          parent, nmem);
    check(compare != NULL, "Compare function can not be NULL.");

    size_t left = left_child(parent);
    size_t right = right_child(parent);
    void *left_addr = locate(array, member_size, nmem, left);
    check(left_addr != NULL, "compare children for parent %lu without left child",
          parent);

    void *right_addr = locate(array, member_size, nmem, right);
    check(right_addr != NULL, "compare children for parent %lu without right child",
          parent);

    return compare(left_addr, right_addr) > 0 ? left : right;
error:
    return 0;
}


/**
 * heapsort sorts an array `array[1..nmem]` of size `member_size` into ascending order according
 * to a compare function that the caller provides.
 *
 *  @param array void * array of items
 *  @param nmem size_t the number of members of the array
 *  @param member_size size_t the member_size of one member of the array
 *  @param compare compare_t the compare function
 *  @return int zero if all processed ok, non zero if not.
 *
 */

int heapsort(void * const array, const size_t nmem, const size_t member_size, compare_t compare)
{
    return __heapsort(array, nmem, member_size, compare, NULL);
}

int __heapsort(void * const array, const size_t nmem, const size_t member_size, compare_t compare,
               void (*debug_print)(void * const array, const size_t member_size, const void *temp))
{

#ifdef NDEBUG
    /* debug_print is only in use if NDEBUG is not defined */
    (void)(debug_print);
#endif

    /** the phase of the sort or an error phase if one was encountered */
    phase_t phase;
    /** length of the sub array (the part that is not sorted yet). */
    size_t heap_size;

    /** The current position (one based) of the vacancy to be filled */
    size_t vacancy_pos;
    /** The temp value that stores the element being evaluated. */
    char *temp = malloc(member_size);

    check(temp != NULL, "Allocate temp failed.");
    check(compare != NULL, "Comparing function should not be NULL.");
    check(array != NULL, "Array to be sorted should not be NULL.");

    /* check nmem: if zero or 1, return immediately (no sort necessary) */
    if (nmem < 2)
    {
        if (temp) free(temp);
        return 0;
    }

    phase = HIRING_PHASE;
    vacancy_pos = (nmem >> 1) + 1;
    heap_size = nmem;

#ifndef NDEBUG
    if (debug_print) debug_print(array, nmem, (void *)temp);
#endif

    /* heap building phase */
    while (--vacancy_pos > 0)
    {
        phase = new_hire(array, member_size, nmem, vacancy_pos, temp);
        check(phase != ERROR_PHASE, "Hiring failed");
        /* Promotion and evaluation of new hire */
        int ret = heapify(array, member_size, heap_size, vacancy_pos, temp, compare);
        check(ret == 0, "Promotion and evaluation failed");
    }

#ifndef NDEBUG
    if (debug_print) debug_print(array, nmem, (void *)temp);
#endif

    /* Sorting phase */
    size_t boss = 1;		/* we always retire the boss (largest) */
    phase = retire_boss(array, member_size, heap_size, temp);
    check(phase != ERROR_PHASE, "Retirement failed.");
    while (--heap_size > 0)
    {
        int ret = heapify(array, member_size, heap_size, boss, temp, compare);
        check(ret == 0, "Promotion and evaluation failed");
        phase = retire_boss(array, member_size, heap_size, temp);
        check(phase != ERROR_PHASE, "Retirement failed.");
    }
    /* Once the heap is empty, the last boss retired is already where he should be */

#ifndef NDEBUG
    if (debug_print) debug_print(array, nmem, (void *)temp);
#endif

    if (temp) free(temp);
    return 0;
error:
    if (temp) free(temp);
    return -1;
}

/**
 * Creates the heap from a filled array accepting new hires.
 *
 * @param array void * The pointer to array
 * @param member_size  size_t The member_size of one member
 * @param vacancy_pos size_t The vacancy available to the new hire
 * @param temp void * The pointer to the temp
 * @return phase_t The RETIREMENT_PHASE mostly, finally the SORT_FINISHED_PHASE. The ERROR_PHASE
 *                 only on error.
 *
 */
phase_t new_hire(void * array, size_t member_size, size_t nmem, size_t vacancy_pos, void *temp)
{
    check(array != NULL, "Array must not be NULL.");
    check(member_size > 0, "member_size of a member should not be zero");
    check(vacancy_pos > 0, "Vacancy position can not be zero");
    check(temp != NULL, "temp can not be NULL.");

    int r = heap_move_mem(member_size, temp, locate(array, member_size, nmem, vacancy_pos));
    check(r == 0, "heap_move_mem from vacancy_pos %lu to temp failed.", vacancy_pos);

    phase_t phase = vacancy_pos > 1 ? HIRING_PHASE : RETIREMENT_PHASE;
    return phase;
error:
    return ERROR_PHASE;
}

/**
 * Retires the largest element to the end of the heap.
 *
 * @param array void * The pointer to array
 * @param member_size  size_t The member_size of one member
 * @param heap_size size_t The number of members in the heap.
 * @param temp void * The pointer to the temp
 * @return phase_t The RETIREMENT_PHASE mostly, finally the SORT_FINISHED_PHASE. The ERROR_PHASE
 *                 only on error.
 */
phase_t retire_boss(void *array, size_t member_size,
                    size_t heap_size, void *temp)
{
    check(array != NULL, "Array must not be NULL.");
    check(temp != NULL, "temp can not be NULL.");
    check(heap_size != 0, "Heap can not be empty");

    size_t nmem = heap_size;

    if (heap_size == 1)
    {
        /* Nothing to do: one member is sorted by definition */
        return SORT_FINISHED_PHASE;
    }
    /* Clear a space at the end of the sub array */
    int r = heap_move_mem(member_size, temp, locate(array, member_size, nmem, heap_size));
    check(r == 0, "heap_move_mem from last position %lu to from failed.", heap_size);

    /* Retire the top of the heap into the last space we just created. */
    r = heap_move_mem(member_size,
                      locate(array, member_size, nmem, heap_size),
                      locate(array, member_size, nmem, 1));
    check(r == 0, "heap_move_mem from position 1 to end of heap %lu failed.", heap_size);

    return RETIREMENT_PHASE;
error:
    return ERROR_PHASE;
}

/**
 * This function turns the array back to a heap.
 *
 * Using a promote and evaluate strategy the underlings are promoted
 * until the temp fits in.
 *
 * Precondition: The heap has one vacancy, indicated by vacancy_pos. Temp has a
 *				 new hire or the displaced employee by retirement of the boss.
 * Postcondition: The heap is one shorter (boss retired) and a fully a heap again
 *					Hence the name heapify.
 *
 * @param array void * The pointer to array
 * @param member_size  size_t The member_size of one member
 * @param heap_size size_t The number of members in the heap.
 * @param promotion_pos size_t The position that was vacated by either retirement or hiring
 * @param temp void * The pointer to the temp
 * @param compare compare_t the compare function provided by the caller.
 * @return int Zero if all is ok, -1 if not ok.
 *
 */
int heapify(void *array, size_t member_size, size_t heap_size,
            size_t promotion_pos, void *temp, compare_t compare)
{
    while (has_left_child(promotion_pos, heap_size))
    {
        /* save the largest position of the children */

        size_t largest_child =
            has_right_child(promotion_pos, heap_size) ?
            compare_children(array, member_size, heap_size, promotion_pos, compare) :
            left_child(promotion_pos);
        check(largest_child > 0, "Largest child can not be position zero.");

        /* Compare temp to the largest child value */
        if (compare(temp, locate(array, member_size, heap_size, largest_child)) >= 0)
        {
            /* We are done: this is where temp fits in. */
            break;
        }
        /* temp is smaller, promote the largest child and set the new promotion position */
        int r = heap_move_mem(member_size,
                              locate(array, member_size, heap_size, promotion_pos),
                              locate(array, member_size, heap_size, largest_child));
        check(r == 0, "Memory heap_move_mem during demotion failed.");
        promotion_pos = largest_child;
    }
    /* Put temp in its place */
    int r = heap_move_mem(member_size, locate(array, member_size, heap_size, promotion_pos), temp);
    check(r == 0, "Memory copy of temp into place failed.");

    return 0;
error:
    return -1;
}
