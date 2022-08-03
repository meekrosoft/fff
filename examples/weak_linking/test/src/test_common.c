#include "test_common.h"
#include <assert.h>
#include <stdio.h>

char GLOBAL_TEST_bus_read_ret[32];


void spoof_runtime_error( const char* info )
{
    fprintf(stderr, "Runtime error: %s\n", info );
    assert(0);
}

bool spoof_bus_read_write( uint8_t dev, uint8_t registry, uint8_t* buffer, int len, bool assume_echo )
{
    memcpy( buffer, GLOBAL_TEST_bus_read_ret, len );
    fprintf(stderr, "bus spoof %d %d\n", (int)dev, (int)registry );
    return true;
}

void init_tests()
{
    memset( GLOBAL_TEST_bus_read_ret, 0x00, sizeof(GLOBAL_TEST_bus_read_ret));
    FFF_RESET_HISTORY();

    RESET_FAKE(bus_read_write);
    RESET_FAKE(bus_write);
    RESET_FAKE(runtime_error);

    runtime_error_fake.custom_fake = spoof_runtime_error;
    bus_read_write_fake.custom_fake = spoof_bus_read_write;
    bus_write_fake.return_val = true;
}