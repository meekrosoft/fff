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

#ifndef TEST_WITH_CALLING_CONVENTIONS
FAKE_VOID_FUNC(voidfunc1, int);
FAKE_VOID_FUNC(voidfunc2, char, char);
FAKE_VOID_FUNC(voidfunc1outparam, char *);
FAKE_VALUE_FUNC(long, longfunc0);
FAKE_VOID_FUNC_VARARG(voidfunc3var, const char *, int, ...);
FAKE_VALUE_FUNC_VARARG(int, valuefunc3var, const char *, int, ...);
FAKE_VOID_FUNC(voidfunc20, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int);
FAKE_VALUE_FUNC(int, valuefunc20, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int);
#else
FAKE_VOID_FUNC(__cdecl, voidfunc1, int);
FAKE_VOID_FUNC(__cdecl, voidfunc2, char, char);
FAKE_VOID_FUNC(__cdecl, voidfunc1outparam, char *);
FAKE_VALUE_FUNC(long, __cdecl, longfunc0);
FAKE_VOID_FUNC_VARARG(__cdecl, voidfunc3var, const char *, int, ...);
FAKE_VALUE_FUNC_VARARG(int, __cdecl, valuefunc3var, const char *, int, ...);
FAKE_VOID_FUNC(__cdecl, voidfunc20, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int);
FAKE_VALUE_FUNC(int, __cdecl, valuefunc20, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int);
#endif

class FFFTestSuite: public testing::Test
{
public:
    void SetUp()
    {
        RESET_FAKE(voidfunc1);
        RESET_FAKE(voidfunc2);
        RESET_FAKE(longfunc0);
        RESET_FAKE(voidfunc1outparam);
        RESET_FAKE(voidfunc3var);
        RESET_FAKE(valuefunc3var);
        FFF_RESET_HISTORY();
    }
};

TEST_F(FFFTestSuite, default_constants_can_be_overridden)
{
    unsigned sizeCallHistory = (sizeof fff.call_history) / (sizeof fff.call_history[0]);
    ASSERT_EQ(OVERRIDE_CALL_HIST_LEN, sizeCallHistory);
    ASSERT_EQ(OVERRIDE_ARG_HIST_LEN, voidfunc2_fake.arg_history_len);
}

#include "test_cases.include"
