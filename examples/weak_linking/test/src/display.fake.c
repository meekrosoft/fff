#include "display.fake.h"

DEFINE_FAKE_VALUE_FUNC( bool, display_init );
DEFINE_FAKE_VOID_FUNC( display_update, const char* );
