#include <lcthw/bstrlib.h>
#include <lcthw/hashmap_algos.h>
#include "minunit.h"

struct tagbstring test1 = bsStatic("test data 1");
struct tagbstring test2 = bsStatic("test data 2");
struct tagbstring test3 = bsStatic("xest data 3");

char *test_fnv1a()
{
    uint32_t hash = Hashmap_fnv1a_hash(&test1);
    debug("HASH: %u", hash);

    hash = Hashmap_fnv1a_hash(&test2);
    debug("HASH: %u", hash);

    hash = Hashmap_fnv1a_hash(&test3);
    debug("HASH: %u", hash);

    return NULL;
}

char *test_adler32()
{
    uint32_t hash = Hashmap_adler32_hash(&test1);
    debug("HASH: %u", hash);

    hash = Hashmap_adler32_hash(&test2);
    debug("HASH: %u", hash);

    hash = Hashmap_adler32_hash(&test3);
    debug("HASH: %u", hash);

    return NULL;
}

char *test_djb()
{
    uint32_t hash = Hashmap_djb_hash(&test1);
    debug("HASH: %u", hash);

    hash = Hashmap_djb_hash(&test2);
    debug("HASH: %u", hash);

    hash = Hashmap_djb_hash(&test3);
    debug("HASH: %u", hash);

    return NULL;
}

char *all_tests() 
{
    mu_suite_start();

    mu_run_test(test_fnv1a);
    mu_run_test(test_adler32);
    mu_run_test(test_djb);

    return NULL;
}

RUN_TESTS(all_tests);

