#ifndef _AUTOFAKE_DISPLAY_H
#define _AUTOFAKE_DISPLAY_H

#include "fff.h"
#include "display.h"

DECLARE_FAKE_VALUE_FUNC( bool, display_init );
DECLARE_FAKE_VOID_FUNC( display_update, const char* );

#endif  // _AUTOFAKE_DISPLAY_H
