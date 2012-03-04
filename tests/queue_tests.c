#include "minunit.h"
#include <lcthw/queue.h>
#include <assert.h>

char *test_create_destroy()
{
    Queue *queue = Queue_create();
    mu_assert(queue != NULL, "Failed to create queue.");

    Queue_clear(queue); // should do nothing
    Queue_destroy(queue);

    queue = Queue_create();
    mu_assert(queue != NULL, "Failed to make queue #2");
    Queue_clear_destroy(queue);

    return NULL;
}

char *test_send_recv()
{
    Queue *queue = Queue_create();
    mu_assert(queue != NULL, "Failed to create test queue.");
    char *test1 = "test1 data";
    char *test2 = "test2 data";
    char *test3 = "test3 data";

    Queue_send(queue, test1);
    mu_assert(Queue_peek(queue) == test1, "Wrong last value.");

    Queue_send(queue, test2);
    mu_assert(Queue_peek(queue) == test1, "Wrong last value");

    Queue_send(queue, test3);
    mu_assert(Queue_peek(queue) == test1, "Wrong last value.");
    mu_assert(Queue_count(queue) == 3, "Wrong count on push.");

    QUEUE_FOREACH(queue, cur) {
        debug("VAL: %s", (char *)cur->value);
    }

    char *val = Queue_recv(queue);
    mu_assert(val == test1, "Wrong value on pop.");

    val = Queue_recv(queue);
    mu_assert(val == test2, "Wrong value on pop.");

    val = Queue_recv(queue);
    mu_assert(val == test3, "Wrong value on pop.");
    mu_assert(Queue_count(queue) == 0, "Wrong count after pop.");

    Queue_destroy(queue);

    return NULL;
}

char *all_tests() {
    mu_suite_start();

    mu_run_test(test_create_destroy);
    mu_run_test(test_send_recv);

    return NULL;
}

RUN_TESTS(all_tests);

