#ifndef HARDWARE_ABSTRACTION
#define HARDWARE_ABSTRACTION

#include <stdint.h>

#ifndef TESTING
#define IO_MEM_RD8(ADDR)  (*((volatile uint8_t *)(ADDR)))
#define IO_MEM_WR8(ADDR, VAL_8)   (*((volatile uint8_t *)(ADDR)) = (VAL_8))
#else
/* In testing use fake functions to record calls to IO memory */
uint8_t IO_MEM_RD8(uint32_t reg);
void IO_MEM_WR8(uint32_t reg, uint8_t val);
#endif

#endif /* Include guard */
