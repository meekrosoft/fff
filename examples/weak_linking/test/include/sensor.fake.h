
#ifndef _AUTOMOCK_SENSOR_H
#define _AUTOMOCK_SENSOR_H

#include "fff.h"
#include "sensor.h"

DECLARE_FAKE_VALUE_FUNC( bool, sensor_init );
DECLARE_FAKE_VALUE_FUNC( float, sensor_read );

void TEST_sensor_generate_data( char* buffer, float value );

#endif // _AUTOMOCK_SENSOR_H
