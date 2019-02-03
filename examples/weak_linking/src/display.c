#include <string.h>

#include "display.h"
#include "bus.h"

#define DISPLAY_ADDRESS     0xAF
#define DISPLAY_REG_INIT    0x10
#define DISPLAY_REG_UPDATE  0x20

bool display_init()
{
    char init_config[] = { 0xDE, 0xFE, 0x00 };
    bus_read_write( DISPLAY_ADDRESS, DISPLAY_REG_INIT, (uint8_t*)init_config, 3, false );

    if (init_config[2] != 0x10)
        return false;

    return true;
}

void display_update( const char* info )
{
    int len = strlen( info );

    if ( bus_write( DISPLAY_ADDRESS, DISPLAY_REG_UPDATE, (const uint8_t*)info, len, true ) != true )
    {
        runtime_error("display update failed!");
    }
}
