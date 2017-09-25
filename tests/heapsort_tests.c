#include "minunit.h"
#include <lcthw/heapsort.h>
#include <assert.h>

static int empty_array[] = {};
static int one_array[] = { 1 };
static int two_array[] = { 2, 1};
static int int_test_array[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };

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

int struct_cmp(const void *a, const void *b)
{
    test_struct *ia = (test_struct *) a;
    test_struct *ib = (test_struct *) b;
    int ret = ia->some_number < ib->some_number ? -1 : ia->some_number > ib->some_number ? 1 : 0;
    return ret;
}

int int_cmp(const void *a, const void *b)
{
    int *ia = (int *) a;
    int *ib = (int *) b;
    int ret = ia < ib ? -1 : ia > ib ? 1 : 0;
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
    int ret = heapsort(two_array, 2, sizeof(int), int_cmp);
    mu_assert(ret == 0, "Failed to sort a 2 item array");

    return NULL;
}

char *test_10_ints()
{
    int ret_sort = heapsort(int_test_array, 11, sizeof(int), int_cmp);

    for(int i=1; i<11; i++)
    {
        if (int_test_array[i] < int_test_array[i-1])
        {
            log_err("Failed to sort properly %d : %d < %d.",
                    i, int_test_array[i], int_test_array[i-1]);
        }

    }
    mu_assert(ret_sort == 0, "Failed to sort 11 items in int array");

    return NULL;
}

char *test_test_struct()
{
    int ret_struct_sort = heapsort(gen_array, 10, sizeof(test_struct), struct_cmp);

    for(int i=1; i<10; i++)
    {
        if (gen_array[i].some_number < gen_array[i-1].some_number)
        {
            log_err("Failed to sort properly %d : %d < %d.",
                    i, gen_array[i].some_number, gen_array[i-1].some_number);
            log_err("       descr %s < %s.",
                    gen_array[i].descr, gen_array[i-1].descr);
        }
    }
    mu_assert(ret_struct_sort == 0, "Failed to sort struct.");

    return NULL;
}

char *all_tests()
{
    mu_suite_start();

    mu_run_test(test_empty_array);
    mu_run_test(test_one);
    mu_run_test(test_two);
    mu_run_test(test_10_ints);
    mu_run_test(test_test_struct);

    return NULL;
}

RUN_TESTS(all_tests);
