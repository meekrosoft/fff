#ifndef GLOBAL_FAKES_H_
#define GLOBAL_FAKES_H_

#include "../fff.h"
#include "string.h"


////  Imaginary production code header file ///
enum MYBOOL { FALSE = 899, TRUE };
struct MyStruct {
    int x;
    int y;
};
enum MYBOOL enumfunc();
struct MyStruct structfunc();
////  End Imaginary production code header file ///

#ifndef TEST_WITH_CALLING_CONVENTIONS
DECLARE_FAKE_VOID_FUNC(voidfunc1, int);
DECLARE_FAKE_VOID_FUNC(voidfunc2, char, char);
DECLARE_FAKE_VOID_FUNC(voidfunc1outparam, char *);
DECLARE_FAKE_VALUE_FUNC(long, longfunc0);
DECLARE_FAKE_VALUE_FUNC(enum MYBOOL, enumfunc0);
DECLARE_FAKE_VALUE_FUNC(struct MyStruct, structfunc0);
DECLARE_FAKE_VOID_FUNC_VARARG(voidfunc3var, const char *, int, ...);
DECLARE_FAKE_VALUE_FUNC_VARARG(int, valuefunc3var, const char *, int, ...);
DECLARE_FAKE_VOID_FUNC(voidfunc20, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int);
DECLARE_FAKE_VALUE_FUNC(int, valuefunc20, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int);
#else
DECLARE_FAKE_VOID_FUNC(__cdecl, voidfunc1, int);
DECLARE_FAKE_VOID_FUNC(__cdecl, voidfunc2, char, char);
DECLARE_FAKE_VOID_FUNC(__cdecl, voidfunc1outparam, char *);
DECLARE_FAKE_VALUE_FUNC(long, __cdecl, longfunc0);
DECLARE_FAKE_VALUE_FUNC(enum MYBOOL, __cdecl, enumfunc0);
DECLARE_FAKE_VALUE_FUNC(struct MyStruct, __cdecl, structfunc0);
DECLARE_FAKE_VOID_FUNC_VARARG(__cdecl, voidfunc3var, const char *, int, ...);
DECLARE_FAKE_VALUE_FUNC_VARARG(int, __cdecl, valuefunc3var, const char *, int, ...);
DECLARE_FAKE_VOID_FUNC(__cdecl, voidfunc20, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int);
DECLARE_FAKE_VALUE_FUNC(int, __cdecl, valuefunc20, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int);
#endif

#ifndef __cplusplus
#ifndef TEST_WITH_CALLING_CONVENTIONS
DECLARE_FAKE_VALUE_FUNC(int, strlcpy3, char* const, const char* const, const size_t);
#else
DECLARE_FAKE_VALUE_FUNC(int, __cdecl, strlcpy3, char* const, const char* const, const size_t);
#endif
#endif /* __cplusplus */
#endif /* GLOBAL_FAKES_H_ */
