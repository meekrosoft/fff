#include <string.h>

#include "sensor.h"
#include "bus.h"

#define SENSOR_ADDRESS 0xAA
#define SENSOR_REG_READ 0xF0

float LOCAL_buffer[4];

bool sensor_init()
{
    memset( LOCAL_buffer, 0x00, sizeof(LOCAL_buffer));
    return true;
}

float sensor_read()
{
    char data[4] = { 0xFF, 0xFF, 0x00, 0x00 };
    bus_read_write( SENSOR_ADDRESS, SENSOR_REG_READ, (uint8_t*)&data, 4, false );

    if (data[0] != 0x10)
    {
        runtime_error("sensor read failed");
    }

    float ret_value = data[1] + data[2] / 256.0;
    return ret_value;
}
