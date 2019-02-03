#include <stdlib.h>
#include <stdio.h>

#include "display.h"
#include "sensor.h"

void give_error( const char* msg )
{
    char* buffer = runtime_error_nice_print( msg );
    runtime_error( buffer );
    free(buffer);
}

int update_main( void )
{
    if ( !sensor_init() )
        runtime_error("sensor init failed");

    if ( !display_init() )
        runtime_error("display init failed");

    char buffer[32];
    float reading = sensor_read();
    snprintf( buffer, 32, "Sensor: %0.1f", reading );
    display_update( buffer );
    return 0;
}
