#include "global_fakes.h"
#include <string.h> // for memcpy

DEFINE_FAKE_VOID_FUNC1(voidfunc1, int);
DEFINE_FAKE_VOID_FUNC2(voidfunc2, char, char);
DEFINE_FAKE_VALUE_FUNC0(long, longfunc0);
DEFINE_FAKE_VALUE_FUNC0(enum MYBOOL, enumfunc0);
DEFINE_FAKE_VALUE_FUNC0(struct MyStruct, structfunc0);
DEFINE_FAKE_VOID_FUNC3_VARARG(voidfunc3var, const char *, int, ...);
#ifndef __cplusplus
DEFINE_FAKE_VALUE_FUNC3(int, strlcpy3, char* const, const char* const, const size_t);
#endif /* __cplusplus */
DEFINE_FAKE_VOID_FUNC20(voidfunc20, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int);
