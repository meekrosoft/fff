/*
 * fff_test.cpp
 *
 *  Created on: Dec 20, 2010
 *      Author: mlong
 */

// Want to keep the argument history for 13 calls
#define OVERRIDE_ARG_HIST_LEN 13u
#define FFF_ARG_HISTORY_LEN OVERRIDE_ARG_HIST_LEN
// Want to keep the call sequence history for 17 function calls
#define OVERRIDE_CALL_HIST_LEN 17u
#define FFF_CALL_HISTORY_LEN OVERRIDE_CALL_HIST_LEN

#include "../fff.h"
#include <gtest/gtest.h>

DEFINE_FFF_GLOBALS

FAKE_VOID_FUNC(voidfunc1, int);
FAKE_VOID_FUNC(voidfunc2, char, char);
FAKE_VALUE_FUNC(long, longfunc0);
FAKE_VOID_FUNC(voidfunc20, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int);

class FFFTestSuite: public testing::Test
{
public:
    void SetUp()
    {
        RESET_FAKE(voidfunc1);
        RESET_FAKE(voidfunc2);
        RESET_FAKE(longfunc0);
        FFF_RESET_HISTORY();
    }
};

#include "test_cases.include"

TEST_F(FFFTestSuite, default_constants_can_be_overridden)
{
    unsigned sizeCallHistory = (sizeof fff.call_history) / (sizeof fff.call_history[0]);
    ASSERT_EQ(OVERRIDE_CALL_HIST_LEN, sizeCallHistory);
    ASSERT_EQ(OVERRIDE_ARG_HIST_LEN, voidfunc2_fake.arg_history_len);
}

