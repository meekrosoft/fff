#include "error.fake.h"

DEFINE_FAKE_VOID_FUNC( runtime_error, const char* );
DEFINE_FAKE_VALUE_FUNC( char*, runtime_error_nice_print, const char* );
