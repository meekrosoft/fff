#ifndef _AUTOFAKE_ERROR_H
#define _AUTOFAKE_ERROR_H

#include "fff.h"
#include "error.h"

DECLARE_FAKE_VOID_FUNC( runtime_error, const char* );
DECLARE_FAKE_VALUE_FUNC( char*, runtime_error_nice_print, const char* );

#endif  // _AUTOFAKE_ERROR_H
