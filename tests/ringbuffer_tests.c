#include "minunit.h"
#include <lcthw/ringbuffer.h>

static RingBuffer *buffer = NULL;
char data[100] = {'\0'};

char *test_create()
{
    buffer = RingBuffer_create(100);
    mu_assert(buffer != NULL, "Failed to make ringbuffer.");

    return NULL;
}

char *test_destroy()
{
    RingBuffer_destroy(buffer);

    return NULL;
}

char *test_read_write()
{
    mu_assert(RingBuffer_empty(buffer), "Should be empty.");
    mu_assert(!RingBuffer_full(buffer), "Should NOT be full.");

    int rc = RingBuffer_write(buffer, "hello", sizeof("hello"));
    mu_assert(rc == sizeof("hello"), "Failed to write hello.");
    rc = RingBuffer_write(buffer, "Zed", sizeof("Zed"));
    mu_assert(rc == sizeof("Zed"), "Failed to write Zed.");

    mu_assert(!RingBuffer_empty(buffer), "Should NOT be empty.");
    mu_assert(!RingBuffer_full(buffer), "Should NOT be full.");

    rc = RingBuffer_read(buffer, data, sizeof("hello"));
    mu_assert(rc == sizeof("hello"), "Failed to read the hello out.");

    mu_assert(!RingBuffer_empty(buffer), "Should be empty.");
    mu_assert(!RingBuffer_full(buffer), "Should NOT be full.");
 
    rc = RingBuffer_read(buffer, data, sizeof("Zed"));
    mu_assert(rc == sizeof("Zed"), "Failed to read the Zed out.");
    mu_assert(RingBuffer_empty(buffer), "Should be empty.");

    return NULL;
}



char *all_tests() {
    mu_suite_start();

    mu_run_test(test_create);
    mu_run_test(test_read_write);
    mu_run_test(test_destroy);

    return NULL;
}

RUN_TESTS(all_tests);

