#include "minunit.h"
#include <list.h>
#include <assert.h>

char *test_create_destroy()
{
    List *list = List_create();
    mu_assert(list != NULL, "Failed to create list.");

    List_clear(list); // should do nothing
    List_destroy(list);

    list = List_create();
    mu_assert(list != NULL, "Failed to make list #2");
    List_clear_destroy(list);

    return NULL;
}

char *test_push_pop()
{
    List *list = List_create();
    mu_assert(list != NULL, "Failed to create test list.");
    char *test1 = "test1 data";
    char *test2 = "test2 data";
    char *test3 = "test3 data";

    List_push(list, test1);
    mu_assert(List_last(list) == test1, "Wrong last value.");

    List_push(list, test2);
    mu_assert(List_last(list) == test2, "Wrong last value");

    List_push(list, test3);
    mu_assert(List_last(list) == test3, "Wrong last value.");
    mu_assert(List_count(list) == 3, "Wrong count on push.");

    char *val = List_pop(list);
    mu_assert(val == test3, "Wrong value on pop.");

    val = List_pop(list);
    mu_assert(val == test2, "Wrong value on pop.");

    val = List_pop(list);
    mu_assert(val == test1, "Wrong value on pop.");
    mu_assert(List_count(list) == 0, "Wrong count after pop.");

    List_destroy(list);

    return NULL;
}

char *test_shift_unshift()
{
    List *list = List_create();
    mu_assert(list != NULL, "Failed to create list.");
    char *test1 = "test1 data";
    char *test2 = "test2 data";
    char *test3 = "test3 data";

    List_shift(list, test1);
    mu_assert(List_first(list) == test1, "Wrong last value.");

    List_shift(list, test2);
    mu_assert(List_first(list) == test2, "Wrong last value");

    List_shift(list, test3);
    mu_assert(List_first(list) == test3, "Wrong last value.");
    mu_assert(List_count(list) == 3, "Wrong count on shift.");

    char *val = List_unshift(list);
    mu_assert(val == test3, "Wrong value on unshift.");

    val = List_unshift(list);
    mu_assert(val == test2, "Wrong value on unshift.");

    val = List_unshift(list);
    mu_assert(val == test1, "Wrong value on unshift.");
    mu_assert(List_count(list) == 0, "Wrong count after unshift.");

    List_destroy(list);

    return NULL;
}

char *test_remove()
{
    List *list = List_create();
    mu_assert(list != NULL, "Failed to create list.");
    char *test1 = "test1 data";
    char *test2 = "test2 data";
    char *test3 = "test3 data";

    List_shift(list, test1);
    mu_assert(List_first(list) == test1, "Wrong last value.");

    List_shift(list, test2);
    mu_assert(List_first(list) == test2, "Wrong last value");

    List_shift(list, test3);
    mu_assert(List_first(list) == test3, "Wrong last value.");
    mu_assert(List_count(list) == 3, "Wrong count on shift.");

    // we only need to test the middle remove case since push/shift 
    // already tests the other cases

    char *val = List_remove(list, list->first->next);
    mu_assert(val == test2, "Wrong removed element.");
    mu_assert(List_count(list) == 2, "Wrong count after remove.");
    mu_assert(List_first(list) == test3, "Wrong first after remove.");
    mu_assert(List_last(list) == test1, "Wrong last after remove.");

    List_destroy(list);

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

