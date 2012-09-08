extern "C"{
    #include "driver.h"
    #include "registers.h"
}
#include "../../fff.h"
#include <gtest/gtest.h>

DEFINE_FFF_GLOBALS;

FAKE_VOID_FUNC(IO_MEM_WR8, uint32_t, uint8_t);
FAKE_VALUE_FUNC(uint8_t, IO_MEM_RD8, uint32_t);

class DriverTestFFF : public testing::Test
{
public:
    void SetUp()
    {
        RESET_FAKE(IO_MEM_WR8);
        RESET_FAKE(IO_MEM_RD8);
        FFF_RESET_HISTORY();
    }

};

TEST_F(DriverTestFFF, When_writing_Then_writes_data_to_DRIVER_OUTPUT_REGISTER)
{
    driver_write(0x34);
    ASSERT_EQ(1u, IO_MEM_WR8_fake.call_count);
    ASSERT_EQ(0x34u, IO_MEM_WR8_fake.arg1_val);
    ASSERT_EQ(DRIVER_OUTPUT_REGISTER, IO_MEM_WR8_fake.arg0_val);
}


TEST_F(DriverTestFFF, When_reading_data_Then_reads_from_DRIVER_INPUT_REGISTER)
{
    IO_MEM_RD8_fake.return_val = 0x55;
    uint8_t returnedValue = driver_read();
    ASSERT_EQ(1u, IO_MEM_RD8_fake.call_count);
    ASSERT_EQ(0x55u, returnedValue);
    ASSERT_EQ(IO_MEM_RD8_fake.arg0_val, DRIVER_INPUT_REGISTER);
}

TEST_F(DriverTestFFF, Given_revisionB_device_When_initialize_Then_enable_peripheral_before_initializing_it)
{
    // Given
    IO_MEM_RD8_fake.return_val = HARDWARE_REV_B;
    // When
    driver_init_device();

    //Then
    // Gets the hardware revision
    ASSERT_EQ((void*) IO_MEM_RD8, fff.call_history[0]);
    ASSERT_EQ(HARDWARE_VERSION_REGISTER, IO_MEM_RD8_fake.arg0_history[0]);
    // Enables Peripheral
    ASSERT_EQ((void*) IO_MEM_WR8, fff.call_history[1]);
    ASSERT_EQ(DRIVER_PERIPHERAL_ENABLE_REG, IO_MEM_WR8_fake.arg0_history[0]);
    ASSERT_EQ(1, IO_MEM_WR8_fake.arg1_history[0]);
    // Initializes Peripheral
    ASSERT_EQ((void*) IO_MEM_WR8, fff.call_history[2]);
    ASSERT_EQ(DRIVER_PERIPHERAL_INITIALIZE_REG,IO_MEM_WR8_fake.arg0_history[1]);
    ASSERT_EQ(1, IO_MEM_WR8_fake.arg1_history[1]);
}
