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

#include "../fff3.h"
#include <gtest/gtest.h>

FAKE_VOID_FUNC(voidfunc1, int);
FAKE_VOID_FUNC(voidfunc2, char, char);
FAKE_VALUE_FUNC(long, longfunc0);

class FFFTestSuite: public testing::Test
{
public:
    void SetUp()
    {
        RESET_FAKES();
        RESET_HISTORY();
    }
};

// Call counting
TEST_F(FFFTestSuite, when_void_func_never_called_then_callcount_is_zero)
{
    ASSERT_EQ(voidfunc1_fake.call_count, 0u);
}

TEST_F(FFFTestSuite, when_void_func_called_once_then_callcount_is_one)
{
    voidfunc1(66);
    ASSERT_EQ(voidfunc1_fake.call_count, 1u);
}

TEST_F(FFFTestSuite, when_void_func_called_once_and_reset_then_callcount_is_zero)
{
    voidfunc1(66);
    RESET_FAKE(voidfunc1);
    ASSERT_EQ(voidfunc1_fake.call_count, 0u);
}

// Single Argument
TEST_F(FFFTestSuite, when_void_func_with_1_integer_arg_called_then_last_arg_captured)
{
    voidfunc1(77);
    ASSERT_EQ(voidfunc1_fake.arg0_val, 77);
}

TEST_F(FFFTestSuite, when_void_func_with_1_integer_arg_called_twice_then_last_arg_captured)
{
    voidfunc1(77);
    voidfunc1(12);
    ASSERT_EQ(voidfunc1_fake.arg0_val, 12);
}

TEST_F(FFFTestSuite, when_void_func_with_1_integer_arg_called_and_reset_then_captured_arg_is_zero)
{
    voidfunc1(11);
    RESET_FAKE(voidfunc1);
    ASSERT_EQ(voidfunc1_fake.arg0_val, 0);
}

// Two Arguments
TEST_F(FFFTestSuite, when_void_func_with_2_char_args_called_then_last_args_captured)
{
    voidfunc2('a', 'b');
    ASSERT_EQ(voidfunc2_fake.arg0_val, 'a');
    ASSERT_EQ(voidfunc2_fake.arg1_val, 'b');
}

TEST_F(FFFTestSuite, when_void_func_with_2_char_args_called_twice_then_last_args_captured)
{
    voidfunc2('a', 'b');
    voidfunc2('c', 'd');
    ASSERT_EQ(voidfunc2_fake.arg0_val, 'c');
    ASSERT_EQ(voidfunc2_fake.arg1_val, 'd');
}

TEST_F(FFFTestSuite, when_void_func_with_2_char_args_called_and_reset_then_captured_arg_is_zero)
{
    voidfunc2('e', 'f');
    RESET_FAKE(voidfunc2);
    ASSERT_EQ(voidfunc2_fake.arg0_val, 0);
    ASSERT_EQ(voidfunc2_fake.arg1_val, 0);
}

// Argument history
TEST_F(FFFTestSuite, when_fake_func_created_default_history_is_fifty_calls)
{
    ASSERT_EQ(FFF_ARG_HISTORY_LEN, (sizeof voidfunc2_fake.arg0_history) / (sizeof voidfunc2_fake.arg0_history[0]));
    ASSERT_EQ(FFF_ARG_HISTORY_LEN, (sizeof voidfunc2_fake.arg1_history) / (sizeof voidfunc2_fake.arg1_history[0]));
}

TEST_F(FFFTestSuite, when_fake_func_called_then_arguments_captured_in_history)
{
    voidfunc2('g', 'h');
    ASSERT_EQ('g', voidfunc2_fake.arg0_history[0]);
    ASSERT_EQ('h', voidfunc2_fake.arg1_history[0]);
}

TEST_F(FFFTestSuite, argument_history_is_reset_when_RESET_FAKE_called)
{
    //given
    voidfunc2('g', 'h');
    ASSERT_EQ('g', voidfunc2_fake.arg0_history[0]);
    ASSERT_EQ('h', voidfunc2_fake.arg1_history[0]);
    //when
    RESET_FAKE(voidfunc2);
    //then
    ASSERT_EQ('\0', voidfunc2_fake.arg0_history[0]);
    ASSERT_EQ('\0', voidfunc2_fake.arg1_history[0]);
}

TEST_F(FFFTestSuite, when_fake_func_called_max_times_then_no_argument_histories_dropped)
{
    unsigned int i;
    for (i = 0; i < FFF_ARG_HISTORY_LEN; i++)
    {
        voidfunc2('1' + i, '2' + i);
    }
    ASSERT_EQ(0u, voidfunc2_fake.arg_histories_dropped);
}

TEST_F(FFFTestSuite, when_fake_func_called_max_times_plus_one_then_one_argument_history_dropped)
{
    unsigned int i;
    for (i = 0; i < FFF_ARG_HISTORY_LEN; i++)
    {
        voidfunc2('1' + i, '2' + i);
    }
    voidfunc2('1', '2');
    ASSERT_EQ(1u, voidfunc2_fake.arg_histories_dropped);
    // or in other words..
    ASSERT_GT(voidfunc2_fake.call_count, voidfunc2_fake.arg_history_len);
}

// Return values
TEST_F(FFFTestSuite, value_func_will_return_zero_by_default)
{
    ASSERT_EQ(0l, longfunc0());
}

TEST_F(FFFTestSuite, value_func_will_return_value_given)
{
    longfunc0_fake.return_val = 99l;
    ASSERT_EQ(99l, longfunc0());
}

TEST_F(FFFTestSuite, value_func_will_return_zero_after_reset)
{
    longfunc0_fake.return_val = 99l;
    RESET_FAKE(longfunc0);
    ASSERT_EQ(0l, longfunc0());
}

TEST_F(FFFTestSuite, register_call_macro_registers_one_call)
{
    REGISTER_CALL(longfunc0);
    ASSERT_EQ(call_history[0], (void *)longfunc0);
}

TEST_F(FFFTestSuite, register_call_macro_registers_two_calls)
{
    REGISTER_CALL(longfunc0);
    REGISTER_CALL(voidfunc2);

    ASSERT_EQ(call_history[0], (void *)longfunc0);
    ASSERT_EQ(call_history[1], (void *)voidfunc2);
}

TEST_F(FFFTestSuite, reset_call_history_resets_call_history)
{
    REGISTER_CALL(longfunc0);
    RESET_HISTORY();
    REGISTER_CALL(voidfunc2);

    ASSERT_EQ(1u, call_history_idx);
    ASSERT_EQ(call_history[0], (void *)voidfunc2);
}

TEST_F(FFFTestSuite, call_history_will_not_write_past_array_bounds)
{
    for (unsigned int i = 0; i < FFF_CALL_HISTORY_LEN + 1; i++)
    {
        REGISTER_CALL(longfunc0);
    }
    ASSERT_EQ(FFF_CALL_HISTORY_LEN, call_history_idx);
}

TEST_F(FFFTestSuite, calling_fake_registers_one_call)
{
    longfunc0();
    ASSERT_EQ(call_history_idx, 1u);
    ASSERT_EQ(call_history[0], (void *)longfunc0);
}

TEST_F(FFFTestSuite, return_value_sequences_not_exhausted)
{
    long myReturnVals[3] = { 3, 7, 9 };
    SET_RETURN_SEQ(longfunc0, myReturnVals, 3);
    ASSERT_EQ(myReturnVals[0], longfunc0());
    ASSERT_EQ(myReturnVals[1], longfunc0());
    ASSERT_EQ(myReturnVals[2], longfunc0());
}


TEST_F(FFFTestSuite, return_value_sequences_exhausted)
{
    long myReturnVals[3] = { 3, 7, 9 };
    SET_RETURN_SEQ(longfunc0, myReturnVals, 3);
    ASSERT_EQ(myReturnVals[0], longfunc0());
    ASSERT_EQ(myReturnVals[1], longfunc0());
    ASSERT_EQ(myReturnVals[2], longfunc0());
    ASSERT_EQ(myReturnVals[2], longfunc0());
    ASSERT_EQ(myReturnVals[2], longfunc0());
}

TEST_F(FFFTestSuite, return_value_sequences_reset)
{
    long myReturnVals[3] = { 3, 7, 9 };
    SET_RETURN_SEQ(longfunc0, myReturnVals, 3);
    ASSERT_EQ(myReturnVals[0], longfunc0());
    ASSERT_EQ(myReturnVals[1], longfunc0());
    RESET_FAKE(longfunc0);
    ASSERT_EQ(0, longfunc0());
}


TEST_F(FFFTestSuite, default_constants_can_be_overridden)
{
    unsigned sizeCallHistory = (sizeof call_history) / (sizeof call_history[0]);
    ASSERT_EQ(OVERRIDE_CALL_HIST_LEN, sizeCallHistory);
    ASSERT_EQ(OVERRIDE_ARG_HIST_LEN, voidfunc2_fake.arg_history_len);
}

