#include "minunit.h"
#include <list.h>
#include <assert.h>

char *test_create_destroy()
{
    list_t *list = list_create();
    mu_assert(list != NULL, "Failed to create list.");

    list_clear(list); // should do nothing
    list_destroy(list);

    list = list_create();
    mu_assert(list != NULL, "Failed to make list #2");
    list_clear_destroy(list);

    return NULL;
}

char *test_push_pop()
{
    list_t *list = list_create();
    mu_assert(list != NULL, "Failed to create test list.");
    char *test1 = "test1 data";
    char *test2 = "test2 data";
    char *test3 = "test3 data";

    list_push(list, test1);
    mu_assert(list_last(list) == test1, "Wrong last value.");

    list_push(list, test2);
    mu_assert(list_last(list) == test2, "Wrong last value");

    list_push(list, test3);
    mu_assert(list_last(list) == test3, "Wrong last value.");
    mu_assert(list_count(list) == 3, "Wrong count on push.");

    char *val = list_pop(list);
    mu_assert(val == test3, "Wrong value on pop.");

    val = list_pop(list);
    mu_assert(val == test2, "Wrong value on pop.");

    val = list_pop(list);
    mu_assert(val == test1, "Wrong value on pop.");
    mu_assert(list_count(list) == 0, "Wrong count after pop.");

    list_destroy(list);

    return NULL;
}

char *test_shift_unshift()
{
    list_t *list = list_create();
    mu_assert(list != NULL, "Failed to create list.");
    char *test1 = "test1 data";
    char *test2 = "test2 data";
    char *test3 = "test3 data";

    list_shift(list, test1);
    mu_assert(list_first(list) == test1, "Wrong last value.");

    list_shift(list, test2);
    mu_assert(list_first(list) == test2, "Wrong last value");

    list_shift(list, test3);
    mu_assert(list_first(list) == test3, "Wrong last value.");
    mu_assert(list_count(list) == 3, "Wrong count on shift.");

    char *val = list_unshift(list);
    mu_assert(val == test3, "Wrong value on unshift.");

    val = list_unshift(list);
    mu_assert(val == test2, "Wrong value on unshift.");

    val = list_unshift(list);
    mu_assert(val == test1, "Wrong value on unshift.");
    mu_assert(list_count(list) == 0, "Wrong count after unshift.");

    list_destroy(list);

    return NULL;
}

char *test_remove()
{
    list_t *list = list_create();
    mu_assert(list != NULL, "Failed to create list.");
    char *test1 = "test1 data";
    char *test2 = "test2 data";
    char *test3 = "test3 data";

    list_shift(list, test1);
    mu_assert(list_first(list) == test1, "Wrong last value.");

    list_shift(list, test2);
    mu_assert(list_first(list) == test2, "Wrong last value");

    list_shift(list, test3);
    mu_assert(list_first(list) == test3, "Wrong last value.");
    mu_assert(list_count(list) == 3, "Wrong count on shift.");

    // we only need to test the middle remove case since push/shift 
    // already tests the other cases

    char *val = list_remove(list, list->first->next);
    mu_assert(val == test2, "Wrong removed element.");
    mu_assert(list_count(list) == 2, "Wrong count after remove.");
    mu_assert(list_first(list) == test3, "Wrong first after remove.");
    mu_assert(list_last(list) == test1, "Wrong last after remove.");

    list_destroy(list);

    return NULL;
}


char *all_tests() {
    mu_suite_start();

    mu_run_test(test_create_destroy);
    mu_run_test(test_push_pop);
    mu_run_test(test_shift_unshift);
    mu_run_test(test_remove);

    return NULL;
}

RUN_TESTS(all_tests);

