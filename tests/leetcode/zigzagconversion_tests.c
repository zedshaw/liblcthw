#include "../minunit.h"
#include <assert.h>

// Just Do It for new suit
// the testcase corresponding C algo file
#include <leetcode/zigzagconversion.c>

MU_TEST(convert_PAYPALISHIRING)
{
    char* actual = convert((char*)"PAYPALISHIRING", 3);
    mu_assert_str_eq(actual, "PAHNAPLSIIGYIR");

    actual = convert((char*)"PAYPALISHIRING", 4);
    mu_assert_str_eq(actual, "PINALSIGYAHRPI");

    return NULL;
}

// Just Do It for new suit
//# make clean;make TESTAREA=leetcode TESTSUIT=zigzagconversion
//# make clean;make TESTAREA=leetcode TESTSUIT="\*"
// gdb tests/leetcode/zigzagconversion_tests
RUN_TESTS_EX()