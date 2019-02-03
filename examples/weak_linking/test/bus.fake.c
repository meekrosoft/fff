#include "bus.fake.h"

DEFINE_FAKE_VALUE_FUNC( bool, bus_read_write, uint8_t, uint8_t, uint8_t*, int, bool );
DEFINE_FAKE_VALUE_FUNC( bool, bus_write, uint8_t, uint8_t, const uint8_t*, int, bool );
