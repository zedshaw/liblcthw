#include "minunit.h"
#include <lcthw/ringbuffer.h>

char *test_read_write()
{
    RingBuffer *buffer = RingBuffer_create(100);
    char *data = calloc(1, 100);

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

    RingBuffer_destroy(buffer);
    free(data);

    return NULL;
}



char *all_tests() {
    mu_suite_start();

    mu_run_test(test_read_write);

    return NULL;
}

RUN_TESTS(all_tests);

