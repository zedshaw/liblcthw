#include "minunit.h"
#include <stack.h>
#include <assert.h>

char *test_create_destroy()
{
    Stack_t *stack = stack_create();
    mu_assert(stack != NULL, "Failed to create stack.");

    stack_clear(stack); // should do nothing
    stack_destroy(stack);

    stack = stack_create();
    mu_assert(stack != NULL, "Failed to make stack #2");
    stack_clear_destroy(stack);

    return NULL;
}

char *test_send_recv()
{
    Stack_t *stack = stack_create();
    mu_assert(stack != NULL, "Failed to create test stack.");
    char *test1 = "test1 data";
    char *test2 = "test2 data";
    char *test3 = "test3 data";

    stack_push(stack, test1);
    mu_assert(stack_peek(stack) == test1, "Wrong last value.");

    stack_push(stack, test2);
    mu_assert(stack_peek(stack) == test2, "Wrong last value");

    stack_push(stack, test3);
    mu_assert(stack_peek(stack) == test3, "Wrong last value.");
    mu_assert(stack_count(stack) == 3, "Wrong count on push.");

    STACK_FOREACH(stack, cur) {
        debug("VAL: %s", cur->value);
    }

    char *val = stack_pop(stack);
    mu_assert(val == test3, "Wrong value on pop.");

    val = stack_pop(stack);
    mu_assert(val == test2, "Wrong value on pop.");

    val = stack_pop(stack);
    mu_assert(val == test1, "Wrong value on pop.");
    mu_assert(stack_count(stack) == 0, "Wrong count after pop.");

    stack_destroy(stack);

    return NULL;
}

char *all_tests() {
    mu_suite_start();

    mu_run_test(test_create_destroy);
    mu_run_test(test_send_recv);

    return NULL;
}

RUN_TESTS(all_tests);

