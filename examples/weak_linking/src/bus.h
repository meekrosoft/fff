#pragma once

#include "error.h"

bool bus_read_write( uint8_t dev, uint8_t registry, uint8_t* buffer, int len, bool assume_echo );
bool bus_write( uint8_t dev, uint8_t registry, const uint8_t* buffer, int len, bool assume_echo );
