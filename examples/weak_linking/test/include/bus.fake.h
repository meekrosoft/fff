#ifndef _AUTOFAKE_BUS_H
#define _AUTOFAKE_BUS_H

#include "fff.h"
#include "bus.h"

DECLARE_FAKE_VALUE_FUNC( bool, bus_read_write, uint8_t, uint8_t, uint8_t*, int, bool );
DECLARE_FAKE_VALUE_FUNC( bool, bus_write, uint8_t, uint8_t, const uint8_t*, int, bool );

#endif  // _AUTOFAKE_BUS_H
