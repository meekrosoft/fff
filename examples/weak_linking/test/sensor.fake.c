#include "sensor.fake.h"

DEFINE_FAKE_VALUE_FUNC( bool, sensor_init );
DEFINE_FAKE_VALUE_FUNC( float, sensor_read );

void TEST_sensor_generate_data( char* buffer, float value )
{
    buffer[0] = 0x10;
    buffer[1] = (int)(value);
    buffer[2] = (value - buffer[1])*255;
}
