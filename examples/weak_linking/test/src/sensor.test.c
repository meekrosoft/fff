#include <stdio.h>

#include "test_common.h"

#include "sensor.h"
#include "sensor.fake.h"

DEFINE_FFF_GLOBALS;


int main(void)
{
    init_tests();

    assert( sensor_init() == true );

    TEST_sensor_generate_data( GLOBAL_TEST_bus_read_ret, 1.5f );
    float value = sensor_read();
    float value_error = value - 1.5f;
    assert( value_error < 0.1f && value_error > -0.1f );
    printf("Test " __FILE__ " ok\n");
    return 0;
}
