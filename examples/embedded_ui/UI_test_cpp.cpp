extern "C"{
#include "UI.h"
#include "SYSTEM.h"
#include "DISPLAY.h"
}
#include <gtest/gtest.h>


#include "../../fff.h"
DEFINE_FFF_GLOBALS;

/* SYSTEM.h */
FAKE_VOID_FUNC(SYSTEM_register_irq, irq_func_t, unsigned int);
/* DISPLAY.h */
FAKE_VOID_FUNC(DISPLAY_init);
FAKE_VOID_FUNC(DISPLAY_clear);
FAKE_VOID_FUNC(DISPLAY_output, char *);
FAKE_VALUE_FUNC(unsigned int, DISPLAY_get_line_capacity);
FAKE_VALUE_FUNC(unsigned int, DISPLAY_get_line_insert_index);

FAKE_VOID_FUNC(button_press_cbk);


class UITests : public testing::Test
{
public:

	void SetUp()
	{
		// Register resets
	    RESET_FAKE(SYSTEM_register_irq);

	    RESET_FAKE(DISPLAY_init)
	    RESET_FAKE(DISPLAY_clear)
	    RESET_FAKE(DISPLAY_output)
	    RESET_FAKE(DISPLAY_get_line_capacity)
	    RESET_FAKE(DISPLAY_get_line_insert_index);

	    RESET_FAKE(button_press_cbk);

		FFF_RESET_HISTORY();
		// non default init
		DISPLAY_get_line_capacity_fake.return_val = 2;
	}
};



/* Tests go here */
TEST_F(UITests, init_will_initialise_display)
{
	UI_init();
	ASSERT_EQ(DISPLAY_init_fake.call_count, 1);
}

TEST_F(UITests, init_will_register_interrupt_gpio2)
{
	UI_init();
    ASSERT_EQ(SYSTEM_register_irq_fake.call_count, 1);
    ASSERT_EQ((void *)SYSTEM_register_irq_fake.arg0_val, (void *)UI_button_irq_handler);
    ASSERT_EQ(SYSTEM_register_irq_fake.arg1_val, IRQ_GPIO_2);
}

TEST_F(UITests, when_no_irq_then_missed_irq_counter_zero)
{
    ASSERT_EQ(UI_get_missed_irqs(), 0);
}

TEST_F(UITests, when_one_irq_and_no_handler_then_missed_irq_counter_one)
{
	UI_button_irq_handler();
    ASSERT_EQ(UI_get_missed_irqs(), 1);
}

TEST_F(UITests, when_one_irq_and_valid_callback_then_missed_irq_counter_zero)
{
	UI_init();
	UI_register_button_cbk(button_press_cbk);
	UI_button_irq_handler();
    ASSERT_EQ(UI_get_missed_irqs(), 0);
}

TEST_F(UITests, when_one_irq_and_valid_callback_then_callback_called)
{
	UI_register_button_cbk(button_press_cbk);
	UI_button_irq_handler();
	ASSERT_EQ(button_press_cbk_fake.call_count, 1);
}

TEST_F(UITests, write_line_outputs_lines_to_display)
{
	char msg[] = "helloworld";
	UI_write_line(msg);
	ASSERT_EQ(DISPLAY_output_fake.call_count, 1);
	ASSERT_EQ(strncmp(DISPLAY_output_fake.arg0_val, msg, 26), 0);
}

TEST_F(UITests, when_no_empty_lines_write_line_clears_screen_and_outputs_lines_to_display)
{
	DISPLAY_get_line_insert_index_fake.return_val = 2;
	char msg[] = "helloworld";

	UI_write_line(msg);

	ASSERT_EQ(DISPLAY_clear_fake.call_count, 1);
	ASSERT_EQ(DISPLAY_output_fake.call_count, 1);
	// Check the order of the calls:  Don't care about the first two:
	// DISPLAY_get_line_capacity and DISPLAY_get_line_insert_index
	ASSERT_EQ(fff.call_history_idx, 4);
	ASSERT_EQ(fff.call_history[2], (void *) DISPLAY_clear);
	ASSERT_EQ(fff.call_history[3], (void *) DISPLAY_output);
}

TEST_F(UITests, when_empty_lines_write_line_doesnt_clear_screen)
{
	// given
	DISPLAY_get_line_insert_index_fake.return_val = 1;
	char msg[] = "helloworld";
	// when
	UI_write_line(msg);
	// then
	ASSERT_EQ(DISPLAY_clear_fake.call_count, 0);
}

TEST_F(UITests, when_string_longer_than_26_then_truncated_string_output)
{
	// given
	char input[] = "abcdefghijklmnopqrstuvwxyz0123456789";
	char expected[] = "abcdefghijklmnopqrstuvwxyz";
	// when
	UI_write_line(input);
	// then
	ASSERT_EQ(strncmp(expected, DISPLAY_output_fake.arg0_val, 37), 0);
}


