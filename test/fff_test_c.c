
// Want to keep the argument history for 13 calls
#define OVERRIDE_ARG_HIST_LEN 13u
#define FFF_ARG_HISTORY_LEN OVERRIDE_ARG_HIST_LEN
// Want to keep the call sequence history for 17 function calls
#define OVERRIDE_CALL_HIST_LEN 17u
#define FFF_CALL_HISTORY_LEN OVERRIDE_CALL_HIST_LEN

#include "../fff.h"
#include "c_test_framework.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>



enum MYBOOL { FALSE = 899, TRUE };
struct MyStruct {
    int x;
    int y;
};


FAKE_VOID_FUNC(voidfunc1, int);
FAKE_VOID_FUNC(voidfunc2, char, char);
FAKE_VALUE_FUNC(long, longfunc0);
FAKE_VALUE_FUNC(enum MYBOOL, enumfunc0);
FAKE_VALUE_FUNC(struct MyStruct, structfunc0);
FAKE_VOID_FUNC3_VARARG(voidfunc3var, char *, int, ...);
FAKE_VALUE_FUNC(int, strlcpy3, char* const, const char* const, const size_t);
FAKE_VOID_FUNC(voidfunc20, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int);

void setup()
{
    RESET_FAKE(voidfunc1);
    RESET_FAKE(voidfunc2);
    RESET_FAKE(longfunc0);
    RESET_FAKE(enumfunc0);
    RESET_FAKE(structfunc0);
    RESET_FAKE(voidfunc3var);
    RESET_FAKE(strlcpy3);
    FFF_RESET_HISTORY();
}


#include "test_cases.include"

TEST_F(FFFTestSuite, default_constants_can_be_overridden)
{
    unsigned sizeCallHistory = (sizeof fff.call_history) / (sizeof fff.call_history[0]);
    ASSERT_EQ(OVERRIDE_CALL_HIST_LEN, sizeCallHistory);
    ASSERT_EQ(OVERRIDE_ARG_HIST_LEN, voidfunc2_fake.arg_history_len);
}

DEFINE_FFF_GLOBALS;
int main()
{
    setbuf(stdout, NULL);
    fprintf(stdout, "-------------\n");
    fprintf(stdout, "Running Tests\n");
    fprintf(stdout, "-------------\n\n");
    fflush(0);

    /* Run tests */
    RUN_TEST(FFFTestSuite, when_void_func_never_called_then_callcount_is_zero);
    RUN_TEST(FFFTestSuite, when_void_func_called_once_then_callcount_is_one);
    RUN_TEST(FFFTestSuite, when_void_func_called_once_and_reset_then_callcount_is_zero);
    RUN_TEST(FFFTestSuite, when_void_func_with_1_integer_arg_called_then_last_arg_captured);
    RUN_TEST(FFFTestSuite, when_void_func_with_1_integer_arg_called_twice_then_last_arg_captured);
    RUN_TEST(FFFTestSuite, when_void_func_with_1_integer_arg_called_and_reset_then_captured_arg_is_zero);
    RUN_TEST(FFFTestSuite, when_void_func_with_2_char_args_called_then_last_args_captured);
    RUN_TEST(FFFTestSuite, when_void_func_with_2_char_args_called_twice_then_last_args_captured);
    RUN_TEST(FFFTestSuite, when_void_func_with_2_char_args_called_and_reset_then_captured_arg_is_zero);
    RUN_TEST(FFFTestSuite, when_fake_func_called_then_const_arguments_captured);

    RUN_TEST(FFFTestSuite, when_fake_func_created_default_history_is_fifty_calls);
    RUN_TEST(FFFTestSuite, when_fake_func_called_then_arguments_captured_in_history);
    RUN_TEST(FFFTestSuite, argument_history_is_reset_when_RESET_FAKE_called);
    RUN_TEST(FFFTestSuite, when_fake_func_called_max_times_then_no_argument_histories_dropped);
    RUN_TEST(FFFTestSuite, when_fake_func_called_max_times_plus_one_then_one_argument_history_dropped);

    RUN_TEST(FFFTestSuite, value_func_will_return_zero_by_default);
    RUN_TEST(FFFTestSuite, value_func_will_return_value_given);
    RUN_TEST(FFFTestSuite, value_func_will_return_zero_after_reset);
    RUN_TEST(FFFTestSuite, register_call_macro_registers_one_call);
    RUN_TEST(FFFTestSuite, register_call_macro_registers_two_calls);
    RUN_TEST(FFFTestSuite, reset_call_history_resets_call_history);
    RUN_TEST(FFFTestSuite, call_history_will_not_write_past_array_bounds);
    RUN_TEST(FFFTestSuite, calling_fake_registers_one_call);

    RUN_TEST(FFFTestSuite, return_value_sequences_not_exhausted);
    RUN_TEST(FFFTestSuite, return_value_sequences_exhausted);
    RUN_TEST(FFFTestSuite, default_constants_can_be_overridden);

    RUN_TEST(FFFTestSuite, can_register_custom_fake);
    RUN_TEST(FFFTestSuite, when_value_custom_fake_called_THEN_it_returns_custom_return_value);

    RUN_TEST(FFFTestSuite, use_vararg_fake_with_different_number_of_arguments);

    RUN_TEST(FFFTestSuite, can_capture_upto_20_arguments_correctly);

    printf("\n-------------\n");
    printf("Complete\n");
    printf("-------------\n\n");

    return 0;
}
