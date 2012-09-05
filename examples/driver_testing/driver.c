

#include "hardware_abstraction.h"
#include "registers.h"

void driver_write(uint8_t val)
{
    IO_MEM_WR8(DRIVER_OUTPUT_REGISTER, val);
}

uint8_t driver_read()
{
    return IO_MEM_RD8(DRIVER_INPUT_REGISTER);
}

void driver_init_device()
{
    uint8_t hw_version = IO_MEM_RD8(HARDWARE_VERSION_REGISTER);
    if(HARDWARE_REV_B == hw_version)
    {
        IO_MEM_WR8(DRIVER_PERIPHERAL_ENABLE_REG, 1);
    }
    IO_MEM_WR8(DRIVER_PERIPHERAL_INITIALIZE_REG, 1);
}
