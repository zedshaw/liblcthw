#include "minunit.h"
#include <sarray.h>

char *test_create_destroy()
{
    int i = 0;
    char test[] = "abracadabra";
    char test2[] = "acadabra";
    SuffixArray *sarry = SuffixArray_create(test, sizeof(test));
    mu_assert(sarry, "Failed to create.");

    int at = SuffixArray_find_suffix(sarry, test2, sizeof(test2));
    mu_assert(at != -1, "Failed to find the suffix.");

    at = SuffixArray_find_suffix(sarry, "yo", sizeof("yo"));
    mu_assert(at == -1, "Should fail to find yo.");

    SuffixArray_destroy(sarry);

    return NULL;
}



char *all_tests() {
    mu_suite_start();

    mu_run_test(test_create_destroy);

    return NULL;
}

RUN_TESTS(all_tests);

