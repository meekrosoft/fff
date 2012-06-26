#include "UI.h"
#include "../../fff.h"
#include "SYSTEM.h"
#include "DISPLAY.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

/* Test Framework :-) */
void setup();
#define TEST_F(SUITE, NAME) void NAME()
#define RUN_TEST(SUITE, TESTNAME) printf(" Running %s.%s: \n", #SUITE, #TESTNAME); setup(); TESTNAME(); printf(" SUCCESS\n");

DEFINE_FFF_GLOBALS;

/* SYSTEM.h */
FAKE_VOID_FUNC2(SYSTEM_register_irq, irq_func_t, unsigned int);
/* DISPLAY.h */
FAKE_VOID_FUNC(DISPLAY_init);
FAKE_VOID_FUNC(DISPLAY_clear);
FAKE_VOID_FUNC(DISPLAY_output, char *);
FAKE_VALUE_FUNC(unsigned int, DISPLAY_get_line_capacity);
FAKE_VALUE_FUNC(unsigned int, DISPLAY_get_line_insert_index);

FAKE_VOID_FUNC0(button_press_cbk);



/* Initialializers called for every test */
void setup()
{
	RESET_FAKE(SYSTEM_register_irq);

	RESET_FAKE(DISPLAY_init)
	RESET_FAKE(DISPLAY_clear)
	RESET_FAKE(DISPLAY_output)
	RESET_FAKE(DISPLAY_get_line_capacity)
	RESET_FAKE(DISPLAY_get_line_insert_index);

	RESET_FAKE(button_press_cbk);

	FFF_RESET_HISTORY();

	DISPLAY_get_line_capacity_fake.return_val = 2;
}

/* Tests go here */
TEST_F(UITests, init_will_initialise_display)
{
	UI_init();
	assert(DISPLAY_init_fake.call_count == 1);
}

TEST_F(UITests, init_will_register_interrupt_gpio2)
{
	UI_init();
    assert(SYSTEM_register_irq_fake.call_count == 1);
    assert(SYSTEM_register_irq_fake.arg0_val == UI_button_irq_handler);
    assert(SYSTEM_register_irq_fake.arg1_val == IRQ_GPIO_2);
}

TEST_F(UITests, when_no_irq_then_missed_irq_counter_zero)
{
    assert(UI_get_missed_irqs() == 0);
}

TEST_F(UITests, when_one_irq_and_no_handler_then_missed_irq_counter_one)
{
	UI_button_irq_handler();
    assert(UI_get_missed_irqs() == 1);
}

TEST_F(UITests, when_one_irq_and_valid_callback_then_missed_irq_counter_zero)
{
	UI_init();
	UI_register_button_cbk(button_press_cbk);
	UI_button_irq_handler();
    assert(UI_get_missed_irqs() == 0);
}

TEST_F(UITests, when_one_irq_and_valid_callback_then_callback_called)
{
	UI_register_button_cbk(button_press_cbk);
	UI_button_irq_handler();
	assert(button_press_cbk_fake.call_count == 1);
}

TEST_F(UITests, write_line_outputs_lines_to_display)
{
	char msg[] = "helloworld";
	UI_write_line(msg);
	assert(DISPLAY_output_fake.call_count == 1);
	assert(strncmp(DISPLAY_output_fake.arg0_val, msg, 26) == 0);
}

TEST_F(UITests, when_no_empty_lines_write_line_clears_screen_and_outputs_lines_to_display)
{
	DISPLAY_get_line_insert_index_fake.return_val = 2;
	char msg[] = "helloworld";

	UI_write_line(msg);

	assert(DISPLAY_clear_fake.call_count == 1);
	assert(DISPLAY_output_fake.call_count == 1);
	// Check the order of the calls:  Don't care about the first two:
	// DISPLAY_get_line_capacity and DISPLAY_get_line_insert_index
	assert(fff.call_history_idx == 4);
	assert(fff.call_history[2] == (void *) DISPLAY_clear);
	assert(fff.call_history[3] == (void *) DISPLAY_output);
}

TEST_F(UITests, when_empty_lines_write_line_doesnt_clear_screen)
{
	// given
	DISPLAY_get_line_insert_index_fake.return_val = 1;
	char msg[] = "helloworld";
	// when
	UI_write_line(msg);
	// then
	assert(DISPLAY_clear_fake.call_count == 0);
}

TEST_F(UITests, when_string_longer_than_26_then_truncated_string_output)
{
	// given
	char input[] = "abcdefghijklmnopqrstuvwxyz0123456789";
	char expected[] = "abcdefghijklmnopqrstuvwxyz";
	// when
	UI_write_line(input);
	// then
	assert(strncmp(expected, DISPLAY_output_fake.arg0_val, 37) == 0);
}

TEST_F(UITests, when_outputting_to_full_display_then_previous_inserted)
{
	// given
	DISPLAY_get_line_insert_index_fake.return_val = 1;
	char oldest[] = "oldest";
	char newest[] = "newest";
	// when
	UI_write_line(oldest);
	UI_write_line(newest);
	// then

	assert(DISPLAY_output_fake.call_count == 2);

	// fills last line
	assert(strncmp(oldest, DISPLAY_output_fake.arg0_history[0], 37) == 0);
	//clears
	assert(DISPLAY_clear_fake.call_count == 1);
	// inserts old line at first
	assert(strncmp(oldest, DISPLAY_output_fake.arg0_history[1], 37) == 0);
	// then inserts new line
	assert(strncmp(newest, DISPLAY_output_fake.arg0_history[2], 37) == 0);
}

int main()
{
	setbuf(stdout, NULL);
	fprintf(stdout, "-------------\n");
	fprintf(stdout, "Running Tests\n");
	fprintf(stdout, "-------------\n\n");
    fflush(0);

	/* Run tests */
    RUN_TEST(UITests, init_will_initialise_display);
    RUN_TEST(UITests, init_will_register_interrupt_gpio2);
    RUN_TEST(UITests, when_no_irq_then_missed_irq_counter_zero);
    RUN_TEST(UITests, when_one_irq_and_no_handler_then_missed_irq_counter_one);
    RUN_TEST(UITests, when_one_irq_and_valid_callback_then_missed_irq_counter_zero);
    RUN_TEST(UITests, when_one_irq_and_valid_callback_then_callback_called);
    RUN_TEST(UITests, write_line_outputs_lines_to_display);
    RUN_TEST(UITests, when_no_empty_lines_write_line_clears_screen_and_outputs_lines_to_display);
    RUN_TEST(UITests, when_empty_lines_write_line_doesnt_clear_screen);
    RUN_TEST(UITests, when_string_longer_than_26_then_truncated_string_output);

    printf("\n-------------\n");
    printf("Complete\n");
	printf("-------------\n\n");

	return 0;
}
