#include <assert.h>
#include "error.h"

char* runtime_error_nice_print( const char* msg )
{
    char* buffer = malloc(512);
    snprintf(buffer, 512, "Got error: %s", msg );
    buffer[511] = 0;
    return buffer;
}
