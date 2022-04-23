#include "../minunit.h"
#include <assert.h>

// Just Do It for new suit
// the testcase corresponding C algo file
#include <leetcode/covertz.c>

MU_TEST(convert_PAYPALISHIRING)
{
    char* actual = convert((char*)"PAYPALISHIRING", 3);
    mu_assert_str_eq(actual, "PAYPALISHIRING");
    return NULL;
}

// Just Do It for new suit
//# make clean;make TESTAREA=leetcode TESTSUIT=covertz
//# make clean;make TESTAREA=leetcode TESTSUIT="\*"
RUN_TESTS_EX()