#include "minunit.h"
#include <lcthw/heapsort.h>
#include <assert.h>
#include <stdbool.h>

static int empty_array[] = {};
static int one_array[] = { 1 };
static int two_array[] = { 2, 1};
static int int_test_in_order[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
static int int_test_reversed[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };

/* prototypes */
void print_ints(void * const array, const size_t nmem, const void *temp);
void print_struct(void * const array, const size_t nmem, const void *temp);

#define MAX_TEST_STRUCT 80
typedef struct
{
    int some_number;
    char descr[MAX_TEST_STRUCT];
} test_struct;

static test_struct gen_array[] =
{
    {10, "ten"},
    {9, "nine"},
    {8, "eight"},
    {7, "seven"},
    {6, "six"},
    {5, "five"},
    {4, "four"},
    {3, "three"},
    {2, "two"},
    {1, "one"},
    {0, "zero"},
};

_Bool is_sorted_ints(int *array, size_t size)
{
    for(size_t i=1; i<size; i++)
    {
        if (array[i] < array[i-1])
        {
            return false;
        }
    }
    return true;
}

_Bool is_sorted_test_struct(test_struct *array, size_t size)
{
    for(size_t i=1; i < size; i++)
    {
        if (array[i].some_number < array[i-1].some_number)
        {
            return false;
        }
    }
    return true;
}


int struct_cmp(const void *a, const void *b)
{
    test_struct *ia = (test_struct *) a;
    test_struct *ib = (test_struct *) b;
    int ret = ia->some_number < ib->some_number ? -1
              : ia->some_number > ib->some_number ? 1 : 0;
    return ret;
}

int int_cmp(const void *a, const void *b)
{
    int *ia = (int *) a;
    int *ib = (int *) b;
    int ret = *ia < *ib ? -1 : *ia > *ib ? 1 : 0;
    return ret;
}

char *test_empty_array()
{
    int ret = heapsort(empty_array, 0, sizeof(int), int_cmp);
    mu_assert(ret == 0, "Failed to sort an empty array");

    return NULL;
}

char *test_one()
{
    int ret = heapsort(one_array, 1, sizeof(int), int_cmp);
    mu_assert(ret == 0, "Failed to sort an array with one element");

    return NULL;
}

char *test_two()
{
    size_t size = 2;
    int ret = heapsort(two_array, size, sizeof(int), int_cmp);
    mu_assert(ret == 0, "Failed to sort a 2 item array");
    mu_assert(is_sorted_ints(int_test_in_order, size), "Array is not sorted properly.");

    return NULL;
}

char *test_ints_in_order()
{
    size_t size = 11;
#ifdef EXTRA_DEBUG
    printf("before sort ints in order -- ");
    print_ints(int_test_in_order, size, 0);
#endif
    int ret_sort = heapsort(int_test_in_order, size, sizeof(int), int_cmp);
#ifdef EXTRA_DEBUG
    printf("after sort ints in order -- ");
    print_ints(int_test_in_order, size, 0);
#endif

    mu_assert(ret_sort == 0, "Failed to sort 11 items in int array");
    mu_assert(is_sorted_ints(int_test_in_order, size), "Array is not sorted properly.");

    return NULL;
}

char *test_ints_reversed()
{
    size_t size = 11;
#ifdef EXTRA_DEBUG
    printf("before sort ints reversed -- ");
    print_ints(int_test_reversed, size, 0);
#endif
    int ret_sort = heapsort(int_test_reversed, size, sizeof(int), int_cmp);
#ifdef EXTRA_DEBUG
    printf("after sort ints reversed -- ");
    print_ints(int_test_reversed, size, 0);
#endif

    mu_assert(ret_sort == 0, "Failed to sort 11 items in int array");
    mu_assert(is_sorted_ints(int_test_reversed, size), "Array is not sorted properly.");

    return NULL;
}

char *test_test_struct()
{
    size_t size = 10;
#ifdef EXTRA_DEBUG
    printf("before sort test_struct -- ");
    print_struct(gen_array, size, 0);
#endif
    int ret_struct_sort = heapsort(gen_array, size, sizeof(test_struct), struct_cmp);
#ifdef EXTRA_DEBUG
    printf("after sort test_struct -- ");
    print_struct(gen_array, size, 0);
#endif

    mu_assert(ret_struct_sort == 0, "Failed to sort struct.");
    mu_assert(is_sorted_test_struct(gen_array, size), "Array is not sorted properly.");

    return NULL;
}

#ifdef EXTRA_DEBUG
void print_ints(void * const array, const size_t nmem, const void *temp)
{
    int *arr = (int *) array;
    printf("array = [");
    for(size_t i=0; i<nmem; i++)
    {
        if (i > 0) printf(", ");
        printf("%d", arr[i]);
    }
    printf("]");
    if (temp) printf(", temp = %d", *(int *)temp);
    printf("\n");
    return;
}
#endif

#ifdef EXTRA_DEBUG
void print_struct(void * const array, const size_t nmem, const void *temp)
{
    test_struct *arr = (test_struct *) array;
    printf("struct array = \n\t[");
    for(size_t i=0; i<nmem; i++)
    {
        if (i > 0) printf(", ");
        if (i > 0 && i%5 == 0) printf("\n\t");
        printf("(%d, %s)", arr[i].some_number, arr[i].descr);
    }
    printf("]");
    if (temp)
    {
        test_struct tmp = *(test_struct *) temp;
        printf(", temp = (%d, %s)", tmp.some_number, tmp.descr);
    }
    printf("\n");
    return;
}
#endif

char *all_tests()
{
    mu_suite_start();

    mu_run_test(test_empty_array);
    mu_run_test(test_one);
    mu_run_test(test_two);
    mu_run_test(test_ints_in_order);
    mu_run_test(test_ints_reversed);
    mu_run_test(test_test_struct);

    return NULL;
}

RUN_TESTS(all_tests);
