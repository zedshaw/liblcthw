#include "../minunit.h"
#include <assert.h>

// Just Do It for new suit
// the testcase corresponding C algo file
#include <leetcode/romantransfer.c>

MU_TEST(III)
{
    int ret = romanToInt((char *)"III");
    mu_assert(ret == 3, "Failed to test romanToInt.");
    return NULL;
}

MU_TEST(MCMXCIV_FAIL)
{
    int ret = romanToInt((char *)"MCMXCIV");
    mu_assert(ret != 1994, "Failed Test");

    return NULL;
}

MU_TEST(MCMXCIV_NULLMSG)
{
    int ret = romanToInt((char *)"MCMXCIV");
    mu_assert(ret != 1994, "");

    return NULL;
}

MU_TEST(MCMXCIV)
{
    int ret = romanToInt((char *)"MCMXCIV");
    mu_assert(ret == 1994, "Failed to test MCMXCIV.");

    return NULL;
}

// Just Do It for new suit
//# make clean;make TESTAREA=leetcode TESTSUIT=romantransfer
//# make clean;make TESTAREA=leetcode TESTSUIT="\*"
RUN_TESTS_EX()