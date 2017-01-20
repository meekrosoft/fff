
#ifndef GLOBAL_FAKES_H_
#define GLOBAL_FAKES_H_

#include "../fff.h"
#include "string.h"


////  Imaginary production code header file ///
void voidfunc1(int);
void voidfunc2(char, char);
void voidfunc1outparam(char *);
long longfunc0();
void voidfunc3var(const char *fmt, int argc, ...);
int valuefunc3var(const char *fmt, int argc, ...);
void voidfunc20(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int);
int valuefunc20(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int);

enum MYBOOL { FALSE = 899, TRUE };
struct MyStruct {
    int x;
    int y;
};
enum MYBOOL enumfunc();
struct MyStruct structfunc();
////  End Imaginary production code header file ///

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

#ifndef __cplusplus
int strlcpy3(char* const, const char* const, const size_t);
DECLARE_FAKE_VALUE_FUNC(int, strlcpy3, char* const, const char* const, const size_t);
#endif /* __cplusplus */
#endif /* GLOBAL_FAKES_H_ */
