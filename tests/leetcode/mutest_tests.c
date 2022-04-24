#include "../minunit.h"
#include <assert.h>

// Just Do It for new suit
// the testcase corresponding C algo file

MU_TEST(example_test)
{
    //char* actual = convert((char*)"PAYPALISHIRING", 3);
    mu_assert_eq(111, 111);
    mu_assert_not_eq(111, 110);
    mu_assert_str_eq("PAYPALISHIRING", "PAYPALISHIRING");
    mu_assert_str_not_eq("PAYPALISHIRING", "PAHNAPLSIIGYIR");
    return NULL;
}

// Just Do It for new suit
//# make clean;make TESTAREA=leetcode TESTSUIT=mutest
//# make clean;make TESTAREA=leetcode TESTSUIT="\*"
RUN_TESTS_EX()