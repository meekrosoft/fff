#include "global_fakes.h"
#include <string.h>

DEFINE_FAKE_VOID_FUNC1(voidfunc1, int);
DEFINE_FAKE_VOID_FUNC2(voidfunc2, char, char);
DEFINE_FAKE_VALUE_FUNC0(long, longfunc0);
DEFINE_FAKE_VALUE_FUNC0(enum MYBOOL, enumfunc0);
DEFINE_FAKE_VALUE_FUNC0(struct MyStruct, structfunc0);
