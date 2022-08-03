#ifndef UI_H_
#define UI_H_

typedef void (*button_cbk_t)(void);

void UI_init();
unsigned int UI_get_missed_irqs();
void UI_button_irq_handler();
void UI_register_button_cbk(button_cbk_t cbk);
void UI_write_line(char *line);

#endif /* UI_H_ */
