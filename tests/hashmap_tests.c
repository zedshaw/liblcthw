#include "minunit.h"
#include <hashmap.h>
#include <assert.h>
#include <bstrlib.h>

char *test_create_destroy()
{
    hashmap_t *map = hashmap_create(NULL, NULL);
    mu_assert(map != NULL, "Failed to create map.");

    hashmap_destroy(map);
    return NULL;
}

static int traverse_called = 0;

int test_traverse_cb(hashmap_node_t *node)
{
    debug("KEY: %s", bdata((bstring)node->key));
    traverse_called++;
    return 0;
}


int test_traverse_fail_cb(hashmap_node_t *node)
{
    debug("KEY: %s", bdata((bstring)node->key));
    traverse_called++;

    if(traverse_called == 2) {
        return 1;
    } else {
        return 0;
    }
}

char *test_get_set_traverse()
{
    bstring test1 = bfromcstr("test data 1");
    bstring test2 = bfromcstr("test data 2");
    bstring test3 = bfromcstr("test data 3");
    bstring expect1 = bfromcstr("THE VALUE 1");
    bstring expect2 = bfromcstr("THE VALUE 2");
    bstring expect3 = bfromcstr("THE VALUE 3");

    hashmap_t *map = hashmap_create(NULL, NULL);
    mu_assert(map != NULL, "Failed to create map.");

    int rc = hashmap_set(map, test1, expect1);
    mu_assert(rc == 0, "Failed to set test1");
    bstring result = hashmap_get(map, test1);
    mu_assert(result == expect1, "Wrong value for test1.");

    rc = hashmap_set(map, test2, expect2);
    mu_assert(rc == 0, "Failed to set test2");
    result = hashmap_get(map, test2);
    mu_assert(result == expect2, "Wrong value for test2.");

    rc = hashmap_set(map, test3, expect3);
    mu_assert(rc == 0, "Failed to set test3");
    result = hashmap_get(map, test3);
    mu_assert(result == expect3, "Wrong value for test3.");

    rc = hashmap_traverse(map, test_traverse_cb);
    mu_assert(rc == 0, "Failed to traverse.");
    mu_assert(traverse_called == 3, "Wrong count traverse.");

    traverse_called = 0;
    rc = hashmap_traverse(map, test_traverse_fail_cb);
    mu_assert(rc == 1, "Failed to traverse.");
    mu_assert(traverse_called == 2, "Wrong count traverse for fail.");

    bstring deleted = (bstring)hashmap_delete(map, test1);
    mu_assert(deleted != NULL, "Got NULL on delete.");
    mu_assert(deleted == expect1, "Should get test1");
    result = hashmap_get(map, test1);
    mu_assert(result == NULL, "Should delete.");

    deleted = (bstring)hashmap_delete(map, test2);
    mu_assert(deleted != NULL, "Got NULL on delete.");
    mu_assert(deleted == expect2, "Should get test2");
    result = hashmap_get(map, test2);
    mu_assert(result == NULL, "Should delete.");

    deleted = (bstring)hashmap_delete(map, test3);
    mu_assert(deleted != NULL, "Got NULL on delete.");
    mu_assert(deleted == expect3, "Should get test3");
    result = hashmap_get(map, test3);
    mu_assert(result == NULL, "Should delete.");

    hashmap_destroy(map);
    bdestroy(test1);
    bdestroy(test2);
    bdestroy(test3);
    bdestroy(expect1);
    bdestroy(expect2);
    bdestroy(expect3);

    return NULL;
}

char *all_tests() {
    mu_suite_start();

    mu_run_test(test_create_destroy);
    mu_run_test(test_get_set_traverse);

    return NULL;
}

RUN_TESTS(all_tests);

