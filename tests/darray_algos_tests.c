#include "minunit.h"
#include <lcthw/darray_algos.h>

int testcmp(char **a, char **b)
{
    return strcmp(*a, *b);
}

DArray *create_words()
{
    DArray *result = DArray_create(0, 5);
    char *words[] = {"asdfasfd", "werwar", "13234", "asdfasfd", "oioj"};
    int i = 0;

    for(i = 0; i < 5; i++) {
        DArray_push(result, words[i]);
    }

    return result;
}

int is_sorted(DArray *array)
{
    int i = 0;

    for(i = 0; i < DArray_count(array) - 1; i++) {
        if(strcmp(DArray_get(array, i), DArray_get(array, i+1)) > 0) {
            return 0;
        }
    }

    return 1;
}

char *test_qsort()
{
    DArray *words = create_words();
    mu_assert(!is_sorted(words), "Words should start not sorted.");

    DArray_qsort(words, (DArray_compare)testcmp);
    mu_assert(is_sorted(words), "qsort failed");

    DArray_destroy(words);
    return NULL;
}

char *test_heapsort()
{
    DArray *words = create_words();

    DArray_heapsort(words, (DArray_compare)testcmp);
    mu_assert(is_sorted(words), "heapsort failed");

    DArray_destroy(words);
    return NULL;
}

char *test_mergesort()
{
    DArray *words = create_words();

    DArray_mergesort(words, (DArray_compare)testcmp);
    mu_assert(is_sorted(words), "mergesort failed");

    DArray_destroy(words);
    return NULL;
}


char * all_tests() 
{
    mu_suite_start();

    mu_run_test(test_qsort);
    mu_run_test(test_heapsort);
    mu_run_test(test_mergesort);

    return NULL;
}

RUN_TESTS(all_tests);

