#pragma once

uint16_t* BUS_REGISTER_BASE = (uint16_t*)0xDEADBEEF;

typedef struct
{
    uint16_t action;
    uint16_t reg;
    uint8_t  input;
    uint8_t  output
} BusDevice;

BusDevice* BUS = (BusDevice*)BUS_REGISTER_BASE;

bool bus_read_write( uint8_t dev, uint8_t registry, uint8_t* buffer, int len, bool assume_echo )
{
    // Something that we dont want to run, since the emulation would be hard.
    BUS->action = 0x01;
    BUS->reg = registry;
    for ( int loop =0 ; loop < len; loop ++ )
    {
        char output = buffer[loop];
        BUS->output  = output;
        buffer[loop] = BUS->input;

        if ( ( assume_echo ) && ( buffer[loop] != output ) )
            return false;
    }
    return true;
}
