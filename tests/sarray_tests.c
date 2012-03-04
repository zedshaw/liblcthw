#include "minunit.h"
#include <lcthw/sarray.h>

static SuffixArray *sarry = NULL;
static char test[] = "abracadabra";
static char test2[] = "acadabra";

char *test_create()
{
    sarry = SuffixArray_create(test, sizeof(test));
    mu_assert(sarry, "Failed to create.");

    return NULL;
}

char *test_destroy()
{
    SuffixArray_destroy(sarry);

    return NULL;
}

char *test_find_suffix()
{
    int at = SuffixArray_find_suffix(sarry, test2, sizeof(test2));
    mu_assert(at != -1, "Failed to find the suffix.");

    at = SuffixArray_find_suffix(sarry, "yo", sizeof("yo"));
    mu_assert(at == -1, "Should fail to find yo.");

    return NULL;
}



char *all_tests() {
    mu_suite_start();

    mu_run_test(test_create);
    mu_run_test(test_find_suffix);
    mu_run_test(test_destroy);

    return NULL;
}

RUN_TESTS(all_tests);

