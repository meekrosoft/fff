/*
 * fff_test.cpp
 *
 *  Created on: Dec 20, 2010
 *      Author: mlong
 */

#include "../fff.h"
#include <gtest/gtest.h>

FAKE_VOID_FUNC1(voidfunc1, int);
FAKE_VOID_FUNC2(voidfunc2, char, char);
FAKE_VALUE_FUNC0(long, longfunc0);

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
    ASSERT_EQ(voidfunc1_call_count, 0u);
}

TEST_F(FFFTestSuite, when_void_func_called_once_then_callcount_is_one)
{
    voidfunc1(66);
    ASSERT_EQ(voidfunc1_call_count, 1u);
}

TEST_F(FFFTestSuite, when_void_func_called_once_and_reset_then_callcount_is_zero)
{
    voidfunc1(66);
    RESET_FAKE(voidfunc1);
    ASSERT_EQ(voidfunc1_call_count, 0u);
}

// Single Argument
TEST_F(FFFTestSuite, when_void_func_with_1_integer_arg_called_then_last_arg_captured)
{
    voidfunc1(77);
    ASSERT_EQ(voidfunc1_arg0_val, 77);
}

TEST_F(FFFTestSuite, when_void_func_with_1_integer_arg_called_twice_then_last_arg_captured)
{
    voidfunc1(77);
    voidfunc1(12);
    ASSERT_EQ(voidfunc1_arg0_val, 12);
}

TEST_F(FFFTestSuite, when_void_func_with_1_integer_arg_called_and_reset_then_captured_arg_is_zero)
{
    voidfunc1(11);
    RESET_FAKE(voidfunc1);
    ASSERT_EQ(voidfunc1_arg0_val, 0);
}

// Two Arguments
TEST_F(FFFTestSuite, when_void_func_with_2_char_args_called_then_last_args_captured)
{
    voidfunc2('a', 'b');
    ASSERT_EQ(voidfunc2_arg0_val, 'a');
    ASSERT_EQ(voidfunc2_arg1_val, 'b');
}

TEST_F(FFFTestSuite, when_void_func_with_2_char_args_called_twice_then_last_args_captured)
{
    voidfunc2('a', 'b');
    voidfunc2('c', 'd');
    ASSERT_EQ(voidfunc2_arg0_val, 'c');
    ASSERT_EQ(voidfunc2_arg1_val, 'd');
}

TEST_F(FFFTestSuite, when_void_func_with_2_char_args_called_and_reset_then_captured_arg_is_zero)
{
    voidfunc2('e', 'f');
    RESET_FAKE(voidfunc2);
    ASSERT_EQ(voidfunc2_arg0_val, 0);
    ASSERT_EQ(voidfunc2_arg1_val, 0);
}

// Argument history
TEST_F(FFFTestSuite, when_fake_func_created_default_history_is_ten_calls)
{
    ASSERT_EQ(10u, (sizeof voidfunc2_arg0_history) / (sizeof voidfunc2_arg0_history[0]));
    ASSERT_EQ(10u, (sizeof voidfunc2_arg1_history) / (sizeof voidfunc2_arg1_history[0]));
}

TEST_F(FFFTestSuite, when_fake_func_called_then_arguments_captured_in_history)
{
    voidfunc2('g', 'h');
    ASSERT_EQ('g', voidfunc2_arg0_history[0]);
    ASSERT_EQ('h', voidfunc2_arg1_history[0]);
}

TEST_F(FFFTestSuite, argument_history_is_reset_when_RESET_FAKE_called)
{
    //given
    voidfunc2('g', 'h');
    ASSERT_EQ('g', voidfunc2_arg0_history[0]);
    ASSERT_EQ('h', voidfunc2_arg1_history[0]);
    //when
    RESET_FAKE(voidfunc2);
    //then
    ASSERT_EQ('\0', voidfunc2_arg0_history[0]);
    ASSERT_EQ('\0', voidfunc2_arg1_history[0]);
}

TEST_F(FFFTestSuite, when_fake_func_called_max_times_then_no_argument_histories_dropped)
{
    int i;
    for (i = 0; i < 10; i++)
    {
        voidfunc2('1' + i, '2' + i);
    }
    ASSERT_EQ(0u, voidfunc2_arg_histories_dropped);
}

TEST_F(FFFTestSuite, when_fake_func_called_max_times_plus_one_then_one_argument_history_dropped)
{
    int i;
    for (i = 0; i < 10; i++)
    {
        voidfunc2('1' + i, '2' + i);
    }
    voidfunc2('1', '2');
    ASSERT_EQ(1u, voidfunc2_arg_histories_dropped);
}

// Return values
TEST_F(FFFTestSuite, value_func_will_return_zero_by_default)
{
    ASSERT_EQ(0l, longfunc0());
}

TEST_F(FFFTestSuite, value_func_will_return_value_given)
{
    longfunc0_return_val = 99l;
    ASSERT_EQ(99l, longfunc0());
}

TEST_F(FFFTestSuite, value_func_will_return_zero_after_reset)
{
    longfunc0_return_val = 99l;
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
    for (unsigned int i = 0; i < MAX_CALL_HISTORY + 1; i++)
    {
        REGISTER_CALL(longfunc0);
    }
    ASSERT_EQ(MAX_CALL_HISTORY, call_history_idx);
}

TEST_F(FFFTestSuite, calling_fake_registers_one_call)
{
    longfunc0();
    ASSERT_EQ(call_history_idx, 1u);
    ASSERT_EQ(call_history[0], (void *)longfunc0);
}
