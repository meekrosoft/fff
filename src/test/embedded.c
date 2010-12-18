


void DISPLAY_init();
void DISPLAY_clear();
int DISPLAY_get_line_capacity();
int DISPLAY_get_line_insert_index();
void DISPLAY_output_message(char * message);
unsigned char DISPLAY_get_pixel(unsigned int x, unsigned int y);

void UI_init()
{
	DISPLAY_init();
}

void UI_greet(char * name)
{
	if(DISPLAY_get_line_capacity() == DISPLAY_get_line_insert_index())
	{
		DISPLAY_clear();
	}
	DISPLAY_output_message(name);
}

void UI_greet_multiple_times(char * name, int times)
{
	int i;
	for(i = 0; i < times; i++)
	{
		UI_greet(name);
	}
}



