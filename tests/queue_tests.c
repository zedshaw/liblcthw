#include "minunit.h"
#include <queue.h>
#include <assert.h>

char *test_create_destroy()
{
    queue_t *queue = queue_create();
    mu_assert(queue != NULL, "Failed to create queue.");

    queue_clear(queue); // should do nothing
    queue_destroy(queue);

    queue = queue_create();
    mu_assert(queue != NULL, "Failed to make queue #2");
    queue_clear_destroy(queue);

    return NULL;
}

char *test_send_recv()
{
    queue_t *queue = queue_create();
    mu_assert(queue != NULL, "Failed to create test queue.");
    char *test1 = "test1 data";
    char *test2 = "test2 data";
    char *test3 = "test3 data";

    queue_send(queue, test1);
    mu_assert(queue_peek(queue) == test1, "Wrong last value.");

    queue_send(queue, test2);
    mu_assert(queue_peek(queue) == test1, "Wrong last value");

    queue_send(queue, test3);
    mu_assert(queue_peek(queue) == test1, "Wrong last value.");
    mu_assert(queue_count(queue) == 3, "Wrong count on push.");

    QUEUE_FOREACH(queue, cur) {
        debug("VAL: %s", cur->value);
    }

    char *val = queue_recv(queue);
    mu_assert(val == test1, "Wrong value on pop.");

    val = queue_recv(queue);
    mu_assert(val == test2, "Wrong value on pop.");

    val = queue_recv(queue);
    mu_assert(val == test3, "Wrong value on pop.");
    mu_assert(queue_count(queue) == 0, "Wrong count after pop.");

    queue_destroy(queue);

    return NULL;
}

char *all_tests() {
    mu_suite_start();

    mu_run_test(test_create_destroy);
    mu_run_test(test_send_recv);

    return NULL;
}

RUN_TESTS(all_tests);

