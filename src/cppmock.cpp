//============================================================================
// Name        : cmock.cpp
// Author      : Mike Long
// Version     :
// Copyright   : Don't steal
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "cppmock.hpp"

extern "C"{
#include "embedded.h"
}

#include <gtest/gtest.h>

FAKE_VOID_FUNC0(DISPLAY_init);
FAKE_VOID_FUNC0(DISPLAY_clear);
FAKE_VOID_FUNC1(DISPLAY_output_message, const char*);
FAKE_VALUE_FUNC0(int, DISPLAY_get_line_capacity);
FAKE_VALUE_FUNC0(int, DISPLAY_get_line_insert_index);


class GreeterTests : public testing::Test
{
public:

	void SetUp()
	{
		// Register resets
		RESET_FAKES();
		// non default init
		DISPLAY_get_line_capacity_return_val = 10;
	}
};

TEST_F(GreeterTests, init_initialises_display)
{
	UI_init();
	ASSERT_EQ(1, DISPLAY_init_call_count);
}

TEST_F(GreeterTests, given_name_when_greet_called_outputs_name)
{
	// given
	char name[] = "mike";
	// when
	UI_greet(name);
	// then
	ASSERT_EQ(1, DISPLAY_output_message_call_count);
	ASSERT_EQ(name, DISPLAY_output_message_arg0_val);
}


TEST_F(GreeterTests, given_name_and_3_times_when_greetmultiple_called_outputs_name_3_times)
{
	// given
	char name[] = "mike";
	// when
	UI_greet_multiple_times(name, 3);
	// then
	ASSERT_EQ(3, DISPLAY_output_message_call_count);
	ASSERT_EQ(name, DISPLAY_output_message_arg0_val);
}

TEST_F(GreeterTests, given_non_full_screen_will_not_reset_display)
{
	char name[] = "mike";
	// given
	DISPLAY_get_line_capacity_return_val = 10;
	DISPLAY_get_line_insert_index_return_val = 0;
	// when
	UI_greet(name);
	// then
	ASSERT_EQ(0, DISPLAY_clear_call_count);
	ASSERT_EQ(1, DISPLAY_output_message_call_count);
}

// Order assumption
TEST_F(GreeterTests, given_full_screen_single_will_reset_display_then_output)
{
	char name[] = "mike";
	// given
	DISPLAY_get_line_capacity_return_val = 1;
	DISPLAY_get_line_insert_index_return_val = 1;
	// when
	UI_greet(name);
	// then
	ASSERT_EQ(1, DISPLAY_clear_call_count);
	ASSERT_EQ(1, DISPLAY_output_message_call_count);
}

// Order assumption
TEST_F(GreeterTests, given_full_screen_multiple_will_reset_display_then_output)
{
	char name[] = "mike";
	// given
	DISPLAY_get_line_capacity_return_val = 4;
	DISPLAY_get_line_insert_index_return_val = 4;
	// when
	UI_greet_multiple_times(name, 1);
	// then
	ASSERT_EQ(1, DISPLAY_clear_call_count);
	ASSERT_EQ(1, DISPLAY_output_message_call_count);
}

