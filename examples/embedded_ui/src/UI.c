#include "UI.h"
#include "DISPLAY.h"
#include "SYSTEM.h"
#include <string.h>

static unsigned int missed_irq_counter;
button_cbk_t button_cbk;


void UI_init()
{
    DISPLAY_init();
    SYSTEM_register_irq(UI_button_irq_handler, IRQ_GPIO_2);
    button_cbk = 0;
    missed_irq_counter = 0;
}

unsigned int UI_get_missed_irqs()
{
	return missed_irq_counter;
}

void UI_button_irq_handler()
{
	if(button_cbk)
	{
		button_cbk();
	}
	else
	{
		missed_irq_counter++;
	}
}

void UI_register_button_cbk(button_cbk_t cbk)
{
	button_cbk = cbk;
}

void UI_write_line(char *line)
{
	static char out[27];
	strncpy(out, line, 26);
	out[26] = '\0';
	if(DISPLAY_get_line_capacity() == DISPLAY_get_line_insert_index())
		DISPLAY_clear();
	DISPLAY_output(out);
}
