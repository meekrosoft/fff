#pragma once

#include <stdint.h>
#include <stdbool.h>

void runtime_error( const char* msg );
char* runtime_error_nice_print( const char* msg );
