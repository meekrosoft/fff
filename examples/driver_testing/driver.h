
#ifndef DRIVER
#define DRIVER

#include <stdint.h>

void driver_write(uint8_t val);
uint8_t driver_read();
void driver_init_device();

#endif /*include guard*/
