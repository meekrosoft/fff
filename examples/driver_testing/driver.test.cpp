extern "C"
{
#include "driver.h"
#include "registers.h"
}
#include "../../fff.h"
#include <gtest/gtest.h>


extern "C"
{
    static uint8_t readVal;
    static int readCalled;
    static uint32_t readRegister;
    uint8_t IO_MEM_RD8(uint32_t reg)
    {
        readRegister = reg;
        readCalled++;
        return readVal;
    }

    static uint32_t writeRegister;
    static uint8_t writeVal;
    static int writeCalled;
    void IO_MEM_WR8(uint32_t reg, uint8_t val)
    {
        writeRegister = reg;
        writeVal = val;
        writeCalled++;
    }
}

TEST(Driver, When_writing_Then_writes_data_to_DRIVER_OUTPUT_REGISTER)
{
    driver_write(0x34);
    ASSERT_EQ(1u, writeCalled);
    ASSERT_EQ(0x34u, writeVal);
    ASSERT_EQ(DRIVER_OUTPUT_REGISTER, writeRegister);
}


TEST(Driver, When_reading_data_Then_reads_from_DRIVER_INPUT_REGISTER)
{
    readVal = 0x55;
    uint8_t returnedValue = driver_read();
    ASSERT_EQ(1u, readCalled);
    ASSERT_EQ(0x55u, returnedValue);
    ASSERT_EQ(readRegister, DRIVER_INPUT_REGISTER);
}

