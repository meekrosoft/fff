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

// Could move this into c_test_framework.h, but it uses the PP iteration macros from fff.h....
#define RUN_INDEXED_TEST(SUITE, TEST_COUNT, TESTNAME, IDX) do { printf(" Running (%d/%d) %s.%s: \n", IDX+1, TEST_COUNT, #SUITE, #TESTNAME); setup(); TESTNAME(); printf(" SUCCESS\n"); } while (0); \


#define RUN_TESTS(SUITE, ...) PP_2PAR_EACH_IDX(RUN_INDEXED_TEST, SUITE, PP_NARG(__VA_ARGS__), __VA_ARGS__)

enum MYBOOL { FALSE = 899, TRUE };
struct MyStruct {
    int x;
    int y;
};

#ifndef TEST_WITH_CALLING_CONVENTIONS
FAKE_VOID_FUNC(voidfunc1, int);
FAKE_VOID_FUNC(voidfunc2, char, char);
FAKE_VOID_FUNC(voidfunc1outparam, char *);
FAKE_VALUE_FUNC(long, longfunc0);
FAKE_VALUE_FUNC(enum MYBOOL, enumfunc0);
FAKE_VALUE_FUNC(struct MyStruct, structfunc0);
FAKE_VOID_FUNC_VARARG(voidfunc3var, const char *, int, ...);
FAKE_VALUE_FUNC_VARARG(int, valuefunc3var, const char *, int, ...);
FAKE_VALUE_FUNC(int, strlcpy3, char* const, const char* const, const size_t);
FAKE_VOID_FUNC(voidfunc20, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int);
FAKE_VALUE_FUNC(int, valuefunc20, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int);
#else
FAKE_VOID_FUNC(__cdecl, voidfunc1, int);
FAKE_VOID_FUNC(__cdecl, voidfunc2, char, char);
FAKE_VOID_FUNC(__cdecl, voidfunc1outparam, char *);
FAKE_VALUE_FUNC(long, __cdecl, longfunc0);
FAKE_VALUE_FUNC(enum MYBOOL, __cdecl, enumfunc0);
FAKE_VALUE_FUNC(struct MyStruct, __cdecl, structfunc0);
FAKE_VOID_FUNC_VARARG(__cdecl, voidfunc3var, char *, int, ...);
FAKE_VALUE_FUNC_VARARG(int, __cdecl, valuefunc3var, char *, int, ...);
FAKE_VALUE_FUNC(int, __cdecl, strlcpy3, char* const, const char* const, const size_t);
FAKE_VOID_FUNC(__cdecl, voidfunc20, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int);
FAKE_VALUE_FUNC(int, __cdecl, valuefunc20, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int);
#endif

#include "test_cases.include"
#include "test_cases_assertions.include"

void setup()
{
    RESET_FAKE(voidfunc1);
    RESET_FAKE(voidfunc2);
    RESET_FAKE(voidfunc1outparam);
    RESET_FAKE(longfunc0);
    RESET_FAKE(enumfunc0);
    RESET_FAKE(structfunc0);
    RESET_FAKE(voidfunc3var);
    RESET_FAKE(valuefunc3var);
    RESET_FAKE(strlcpy3);
    FFF_RESET_HISTORY();
    RESET_FAKE_ASSERTIONS();
}



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
    RUN_TESTS(
        FFFTestSuite, when_void_func_never_called_then_callcount_is_zero,
        when_void_func_called_once_then_callcount_is_one,
        when_void_func_called_once_and_reset_then_callcount_is_zero,
        when_void_func_with_1_integer_arg_called_then_last_arg_captured,
        when_void_func_with_1_integer_arg_called_twice_then_last_arg_captured,
        when_void_func_with_1_integer_arg_called_and_reset_then_captured_arg_is_zero,
        when_void_func_with_2_char_args_called_then_last_args_captured,
        when_void_func_with_2_char_args_called_twice_then_last_args_captured,
        when_void_func_with_2_char_args_called_and_reset_then_captured_arg_is_zero,
        when_fake_func_called_then_const_arguments_captured,

        when_fake_func_created_default_history_is_fifty_calls,
        when_fake_func_called_then_arguments_captured_in_history,
        argument_history_is_reset_when_RESET_FAKE_called,
        when_fake_func_called_max_times_then_no_argument_histories_dropped,
        when_fake_func_called_max_times_plus_one_then_one_argument_history_dropped,

        value_func_will_return_zero_by_default,
        value_func_will_return_value_given,
        value_func_will_return_zero_after_reset,

        register_call_macro_registers_one_call,
        register_call_macro_registers_two_calls,
        reset_call_history_resets_call_history,
        call_history_will_not_write_past_array_bounds,
        calling_fake_registers_one_call,

        return_value_sequences_not_exhausted, return_value_sequences_exhausted,

        default_constants_can_be_overridden,

        can_register_custom_fake,
        when_value_custom_fake_called_THEN_it_returns_custom_return_value,

        use_void_vararg_fake_with_different_number_of_arguments,
        use_value_vararg_fake_with_different_number_of_arguments,
        vararg_custom_fake_sequence_not_exhausted,
        vararg_custom_fake_seq_return_values_saved_in_history,
        vararg_custom_fake_sequence_exhausted,
        vararg_custom_fake_sequence_reset,

        can_capture_upto_20_arguments_correctly,
        value_func_can_capture_upto_20_arguments_correctly,

        variadic_return_values_should_set_return_val_given_single_value,
        variadic_return_values_should_set_return_sequence_given_multiple_values,
        variadic_return_values,
        variadic_return_values_no_array_name_collisions_when_called_repeatedly,
        variadic_return_values_no_assertion_given_single_value,
        variadic_return_values_assertion_valid_given_value_sequence,

#if defined(__GNUC__) && !defined(__STRICT_ANSI__)
        variadic_return_values_assertion_invalid_given_0_length_sequence,
#endif

        assert_calls_assertion_valid_given_match,
        assert_calls_assertion_invalid_when_underestimated,
        assert_calls_assertion_invalid_when_overestimated,

        assert_called_assertion_valid_when_called_once,
        assert_called_assertion_invalid_when_not_called,
        assert_called_assertion_invalid_when_called_more_than_once,

        assert_not_called_assertion_valid_when_not_called,
        assert_not_called_assertion_invalid_when_called_once,
        assert_not_called_assertion_invalid_when_called_more_than_once,

        assert_call_assertion_valid_when_all_args_specified_and_matched,
        assert_call_assertion_valid_when_some_args_specified_and_matched,
        assert_call_assertion_invalid_when_called_more_than_once,
        assert_call_assertion_invalid_given_arg_mismatch,

        assert_nth_call_assertion_valid_given_matching_args,
        assert_nth_call_assertion_invalid_given_arg_mismatch,

        assert_last_call_assertion_valid_given_multiple_calls,
        assert_last_call_assertion_invalid_given_previous_call_args,

        assert_any_call_assertion_valid_given_any_match,
        assert_any_call_assertion_invalid_given_no_match,
        assert_no_call_assertion_invalid_given_any_match,
        assert_no_call_assertion_valid_given_no_match)

    printf("\n-------------\n");
    printf("Complete\n");
    printf("-------------\n\n");

    return 0;
}
