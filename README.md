# Fake Function Framework  (fff)
-----------------------------

[![Build Status](https://travis-ci.org/meekrosoft/fff.svg?branch=master)](https://travis-ci.org/meekrosoft/fff)
[![Build status](https://ci.appveyor.com/api/projects/status/md1gn8cxnjkrkq3b/branch/master?svg=true)](https://ci.appveyor.com/project/wulfgarpro/fff/branch/master)
[![Gitter chat](https://badges.gitter.im/gitterHQ/gitter.png)](https://gitter.im/wulfgarpro/fff?utm_source=share-link&utm_medium=link&utm_campaign=share-link)

- [A Fake Function Framework for C](#a-fake-function-framework-for-c)
- [Hello Fake World!](#hello-fake-world)
- [Capturing Arguments](#capturing-arguments)
- [Return Values](#return-values)
- [Resetting a Fake](#resetting-a-fake)
- [Call History](#call-history)
- [Default Argument History](#default-argument-history)
- [User Defined Argument History](#user-defined-argument-history)
- [Function Return Value Sequences](#function-return-value-sequences)
- [Custom Return Value Delegate](#custom-return-value-delegate)
- [Return Value History](#return-value-history)
- [Variadic Functions](#variadic-functions)
- [Common Questions](#common-questions)
- [Specifying GCC Function Attributes](#specifying-gcc-function-attributes)
- [Find Out More](#find-out-more)
- [Benefits](#benefits)
- [Under the Hood](#under-the-hood)
- [Cheat Sheet](#cheat-sheet)

## A Fake Function Framework for C
fff is a micro-framework for creating fake C functions for tests.  Because life
is too short to spend time hand-writing fake functions for testing.

## Hello Fake World!

Say you are testing an embedded user interface and you have a function that
you want to create a fake for:

```c
// UI.c
...
void DISPLAY_init();
...
```

Here's how you would define a fake function for this in your test suite:

```c
// test.c(pp)
#include "fff.h"
DEFINE_FFF_GLOBALS;
FAKE_VOID_FUNC(DISPLAY_init);
```

And the unit test might look something like this:

```c
TEST_F(GreeterTests, init_initialises_display)
{
    UI_init();
    ASSERT_EQ(DISPLAY_init_fake.call_count, 1);
}
```

So what has happened here?  The first thing to note is that the framework is
header only, all you need to do to use it is download `fff.h` and include
it in your test suite.

The magic is in the `FAKE_VOID_FUNC`.  This
expands a macro that defines a function returning `void`
which has zero arguments.  It also defines a struct
`"function_name"_fake` which contains all the information about the fake.
For instance, `DISPLAY_init_fake.call_count`is incremented every time the faked
function is called.

Under the hood it generates a struct that looks like this:

```c
typedef struct DISPLAY_init_Fake {
    unsigned int call_count;
    unsigned int arg_history_len;
    unsigned int arg_histories_dropped;
    void(*custom_fake)();
} DISPLAY_init_Fake;
DISPLAY_init_Fake DISPLAY_init_fake;
```

## Capturing Arguments

Ok, enough with the toy examples.  What about faking functions with arguments?

```c
// UI.c
...
void DISPLAY_output(char * message);
...
```

Here's how you would define a fake function for this in your test suite:

```c
FAKE_VOID_FUNC(DISPLAY_output, char *);
```

And the unit test might look something like this:

```c
TEST_F(UITests, write_line_outputs_lines_to_display)
{
    char msg[] = "helloworld";
    UI_write_line(msg);
    ASSERT_EQ(DISPLAY_output_fake.call_count, 1);
    ASSERT_EQ(strncmp(DISPLAY_output_fake.arg0_val, msg, 26), 0);
}
```


There is no more magic here, the `FAKE_VOID_FUNC` works as in the
previous example.  The number of arguments that the function takes is calculated,
 and the macro arguments following the function name defines the argument
type (a char pointer in this example).

A variable is created for every argument in the form
`"function_name"fake.argN_val`

## Return Values

When you want to define a fake function that returns a value, you should use the
`FAKE_VALUE_FUNC` macro.  For instance:

```c
// UI.c
...
unsigned int DISPLAY_get_line_capacity();
unsigned int DISPLAY_get_line_insert_index();
...
```

Here's how you would define fake functions for these in your test suite:

```c
FAKE_VALUE_FUNC(unsigned int, DISPLAY_get_line_capacity);
FAKE_VALUE_FUNC(unsigned int, DISPLAY_get_line_insert_index);
```

And the unit test might look something like this:

```c
TEST_F(UITests, when_empty_lines_write_line_doesnt_clear_screen)
{
    // given
    DISPLAY_get_line_insert_index_fake.return_val = 1;
    char msg[] = "helloworld";
    // when
    UI_write_line(msg);
    // then
    ASSERT_EQ(DISPLAY_clear_fake.call_count, 0);
}
```

Of course you can mix and match these macros to define a value function with
arguments, for instance to fake:

```c
double pow(double base, double exponent);
```

you would use a syntax like this:

```c
FAKE_VALUE_FUNC(double, pow, double, double);
```
## Resetting a Fake

Good tests are isolated tests, so it is important to reset the fakes for each
unit test.  All the fakes have a reset function to reset their arguments and
call counts.  It is good practice is to call the reset function for all the
fakes in the setup function of your test suite.

```c
void setup()
{
    // Register resets
    RESET_FAKE(DISPLAY_init);
    RESET_FAKE(DISPLAY_clear);
    RESET_FAKE(DISPLAY_output_message);
    RESET_FAKE(DISPLAY_get_line_capacity);
    RESET_FAKE(DISPLAY_get_line_insert_index);
}
```

You might want to define a macro to do this:

```c
/* List of fakes used by this unit tester */
#define FFF_FAKES_LIST(FAKE)            \
  FAKE(DISPLAY_init)                    \
  FAKE(DISPLAY_clear)                   \
  FAKE(DISPLAY_output_message)          \
  FAKE(DISPLAY_get_line_capacity)       \
  FAKE(DISPLAY_get_line_insert_index)

void setup()
{
  /* Register resets */
  FFF_FAKES_LIST(RESET_FAKE);

  /* reset common FFF internal structures */
  FFF_RESET_HISTORY();
}
```

## Call History
Say you want to test that a function calls functionA, then functionB, then
functionA again, how would you do that?  Well fff maintains a call
history so that it is easy to assert these expectations.

Here's how it works:

```c
FAKE_VOID_FUNC(voidfunc2, char, char);
FAKE_VALUE_FUNC(long, longfunc0);

TEST_F(FFFTestSuite, calls_in_correct_order)
{
    longfunc0();
    voidfunc2();
    longfunc0();

    ASSERT_EQ(fff.call_history[0], (void *)longfunc0);
    ASSERT_EQ(fff.call_history[1], (void *)voidfunc2);
    ASSERT_EQ(fff.call_history[2], (void *)longfunc0);
}
```

They are reset by calling `FFF_RESET_HISTORY();`

## Default Argument History

The framework will by default store the arguments for the last ten calls made
to a fake function.

```c
TEST_F(FFFTestSuite, when_fake_func_called_then_arguments_captured_in_history)
{
    voidfunc2('g', 'h');
    voidfunc2('i', 'j');
    ASSERT_EQ('g', voidfunc2_fake.arg0_history[0]);
    ASSERT_EQ('h', voidfunc2_fake.arg1_history[0]);
    ASSERT_EQ('i', voidfunc2_fake.arg0_history[1]);
    ASSERT_EQ('j', voidfunc2_fake.arg1_history[1]);
}
```

There are two ways to find out if calls have been dropped.  The first is to
check the dropped histories counter:

```c
TEST_F(FFFTestSuite, when_fake_func_called_max_times_plus_one_then_one_argument_history_dropped)
{
    int i;
    for(i = 0; i < 10; i++)
    {
        voidfunc2('1'+i, '2'+i);
    }
    voidfunc2('1', '2');
    ASSERT_EQ(1u, voidfunc2_fake.arg_histories_dropped);
}
```

The other is to check if the call count is greater than the history size:

```c
ASSERT(voidfunc2_fake.arg_history_len < voidfunc2_fake.call_count);
```

The argument histories for a fake function are reset when the `RESET_FAKE`
function is called

## User Defined Argument History

If you wish to control how many calls to capture for argument history you can
override the default by defining it before include the `fff.h` like this:

```c
// Want to keep the argument history for 13 calls
#define FFF_ARG_HISTORY_LEN 13
// Want to keep the call sequence history for 17 function calls
#define FFF_CALL_HISTORY_LEN 17

#include "../fff.h"
```

## Function Return Value Sequences

Often in testing we would like to test the behaviour of sequence of function call
events.  One way to do this with fff is to specify a sequence of return values
with for the fake function.  It is probably easier to describe with an example:

```c
// faking "long longfunc();"
FAKE_VALUE_FUNC(long, longfunc0);

TEST_F(FFFTestSuite, return_value_sequences_exhausted)
{
    long myReturnVals[3] = { 3, 7, 9 };
    SET_RETURN_SEQ(longfunc0, myReturnVals, 3);
    ASSERT_EQ(myReturnVals[0], longfunc0());
    ASSERT_EQ(myReturnVals[1], longfunc0());
    ASSERT_EQ(myReturnVals[2], longfunc0());
    ASSERT_EQ(myReturnVals[2], longfunc0());
    ASSERT_EQ(myReturnVals[2], longfunc0());
}
```

By specifying a return value sequence using the `SET_RETURN_SEQ` macro,
the fake will return the values given in the parameter array in sequence.  When
the end of the sequence is reached the fake will continue to return the last
value in the sequence indefinitely.

## Custom Return Value Delegate

You can specify your own function to provide the return value for the fake. This
is done by setting the `custom_fake` member of the fake.  Here's an example:

```c
#define MEANING_OF_LIFE 42
long my_custom_value_fake(void)
{
    return MEANING_OF_LIFE;
}
TEST_F(FFFTestSuite, when_value_custom_fake_called_THEN_it_returns_custom_return_value)
{
    longfunc0_fake.custom_fake = my_custom_value_fake;
    long retval = longfunc0();
    ASSERT_EQ(MEANING_OF_LIFE, retval);
}
```

### Custom Return Value Delegate Sequences

Say you have a function with an out parameter, and you want it to have a different behaviour
on the first three calls, for example: set the value 'x' to the out parameter on the first call,
the value 'y' to the out parameter on the second call, and the value 'z' to the out parameter
on the third call. You can specify a sequence of custom functions to a non-variadic function
using the `SET_CUSTOM_FAKE_SEQ` macro. Here's an example:

```c
void voidfunc1outparam_custom_fake1(char *a)
{
    *a = 'x';
}

void voidfunc1outparam_custom_fake2(char *a)
{
    *a = 'y';
}

void voidfunc1outparam_custom_fake3(char *a)
{
    *a = 'z';
}

TEST_F(FFFTestSuite, custom_fake_sequence_not_exausthed)
{
    void (*custom_fakes[])(char *) = {voidfunc1outparam_custom_fake1,
                                      voidfunc1outparam_custom_fake2,
                                      voidfunc1outparam_custom_fake3};
    char a = 'a';

    SET_CUSTOM_FAKE_SEQ(voidfunc1outparam, custom_fakes, 3);

    voidfunc1outparam(&a);
    ASSERT_EQ('x', a);
    voidfunc1outparam(&a);
    ASSERT_EQ('y', a);
    voidfunc1outparam(&a);
    ASSERT_EQ('z', a);
}
```

The fake will call your custom functions in the order specified by the `SET_CUSTOM_FAKE_SEQ`
macro. When the last custom fake is reached the fake will keep calling the last custom
fake in the sequence. This macro works much like the `SET_RETURN_SEQ` macro.

## Return Value History

Say you have two functions f1 and f2. f2 must be called to release some resource
allocated by f1, but only in the cases where f1 returns zero. f1 could be
pthread_mutex_trylock and f2 could be pthread_mutex_unlock. fff will
save the history of returned values so this can be easily checked, even when
you use a sequence of custom fakes. Here's a simple example:

    TEST_F(FFFTestSuite, return_value_sequence_saved_in_history)
    {
        long myReturnVals[3] = { 3, 7, 9 };
        SET_RETURN_SEQ(longfunc0, myReturnVals, 3);
        longfunc0();
        longfunc0();
        longfunc0();
        ASSERT_EQ(myReturnVals[0], longfunc0_fake.return_val_history[0]);
        ASSERT_EQ(myReturnVals[1], longfunc0_fake.return_val_history[1]);
        ASSERT_EQ(myReturnVals[2], longfunc0_fake.return_val_history[2]);
    }

You access the returned values in the `return_val_history` field.

## Variadic Functions

You can fake variadic functions using the macros `FAKE_VALUE_FUNC_VARARG`
and `FAKE_VOID_FUNC_VARARG`. For instance:

    FAKE_VALUE_FUNC_VARARG(int, fprintf, FILE *, const char*, ...);

In order to access the variadic parameters from a custom fake function, declare a
`va_list` parameter. For instance, a custom fake for `fprintf()`
could call the real `fprintf()` like this:

    int fprintf_custom(FILE *stream, const char *format, va_list ap) {
      if (fprintf0_fake.return_val < 0) // should we fail?
        return fprintf0_fake.return_val;
      return vfprintf(stream, format, ap);
    }

Just like  [return value delegates](#custom-return-value-delegate-sequences), you can also specify sequences for variadic functions using `SET_CUSTOM_FAKE_SEQ`.
See the test files for examples.

## Common Questions

### How do I specify calling conventions for my fake functions?

fff has a limited capability for enabling specification of Microsoft's Visual C/C++ calling conventions, but this support must be enabled when generating fff's header file `fff.h`.

```bash
ruby fakegen.rb --with-calling-conventions > fff.h
```

By enabling this support, all of fff's fake function scaffolding will necessitate the specification of a calling convention, e.g. `__cdecl` for each VALUE or VOID fake.

Here are some basic examples: take note that the placement of the calling convention being specified is different depending on whether the fake is a VOID or VALUE function.

```c
FAKE_VOID_FUNC(__cdecl, voidfunc1, int);
FAKE_VALUE_FUNC(long, __cdecl, longfunc0);
```

### How do I fake a function that returns a value by reference?

The basic mechanism that fff provides you in this case is the custom_fake field described in the *Custom Return Value Delegate* example above.

You need to create a custom function (e.g. getTime_custom_fake) to produce the output optionally by use of a helper variable (e.g. getTime_custom_now) to retrieve that output from. Then some creativity to tie it all together. The most important part (IMHO) is to keep your test case readable and maintainable.

In case your project uses a C compiler that supports nested functions (e.g. GCC) you can even combine all this in a single unit test function so you can easily oversee all details of the test.

```c
/* The time structure */
typedef struct {
   int hour, min;
} Time;

/* Our fake function */
FAKE_VOID_FUNC(getTime, Time*);

/* A test using the getTime fake function */
TEST_F(FFFTestSuite, when_value_custom_fake_called_THEN_it_returns_custom_output)
{
    Time t;
    Time getTime_custom_now;
    void getTime_custom_fake(Time *now) {
        *now = getTime_custom_now;
    }
    getTime_fake.custom_fake = getTime_custom_fake;

    /* given a specific time */
    getTime_custom_now.hour = 13;
    getTime_custom_now.min  = 05;

    /* when getTime is called */
    getTime(&t);

    /* then the specific time must be produced */
    ASSERT_EQ(t.hour, 13);
    ASSERT_EQ(t.min,  05);
}
```

### How do I fake a function with a function pointer parameter?

Using fff to stub functions that have function pointer parameter can cause problems when trying to stub them. Presented here is an example how to deal with this situation.

If you need to stub a function that has a function pointer parameter, e.g. something like:

```c
/* timer.h */
typedef int timer_handle;
extern int timer_start(timer_handle handle, long delay, void (*cb_function) (int arg), int arg);
```

Then creating a fake like below will horribly fail when trying to compile because the fff macro will internally expand into an illegal variable ```int (*)(int) arg2_val```.

```c
/* The fake, attempt one */
FAKE_VALUE_FUNC(int,
                timer_start,
                timer_handle,
                long,
                void (*) (int argument),
                int);
```

The solution to this problem is to create a bridging type that needs only to be visible in the unit tester. The fake will use that intermediate type. This way the compiler will not complain because the types match.

```c
/* Additional type needed to be able to use callback in fff */
typedef void (*timer_cb) (int argument);

/* The fake, attempt two */
FAKE_VALUE_FUNC(int,
                timer_start,
                timer_handle,
                long,
                timer_cb,
                int);
```

Here are some ideas how to create a test case with callbacks.

```c
/* Unit test */
TEST_F(FFFTestSuite, test_fake_with_function_pointer)
{
    int cb_timeout_called = 0;
    int result = 0;

    void cb_timeout(int argument)
    {
      cb_timeout_called++;
    }

    int timer_start_custom_fake(timer_handle handle,
                          long delay,
                          void (*cb_function) (int arg),
                          int arg)
    {
      if (cb_function) cb_function(arg);
      return timer_start_fake.return_val;
    }

    /* given the custom fake for timer_start */
    timer_start_fake.return_val = 33;
    timer_start_fake.custom_fake = timer_start_custom_fake;

    /* when timer_start is called
     * (actually you would call your own function-under-test
     *  that would then call the fake function)
     */
    result = timer_start(10, 100, cb_timeout, 55);

    /* then the timer_start fake must have been called correctly */
    ASSERT_EQ(result, 33);
    ASSERT_EQ(timer_start_fake.call_count, 1);
    ASSERT_EQ(timer_start_fake.arg0_val,   10);
    ASSERT_EQ(timer_start_fake.arg1_val,   100);
    ASSERT_EQ(timer_start_fake.arg2_val,   cb_timeout); /* callback provided by unit tester */
    ASSERT_EQ(timer_start_fake.arg3_val,   55);

    /* and ofcourse our custom fake correctly calls the registered callback */
    ASSERT_EQ(cb_timeout_called, 1);
}
```

### How do I reuse a fake across multiple test-suites?

fff functions like `FAKE_VALUE_FUNC` will perform both the declaration AND the definition of the fake function and the corresponding data structs. This cannot be placed in a header, since it will lead to multiple definitions of the fake functions.

The solution is to separate declaration and definition of the fakes, and place the declaration into a public header file, and the definition into a private source file.

Here is an example of how it could be done:

```c
/* Public header file */
#include "fff.h"

DECLARE_FAKE_VALUE_FUNC(int, value_function, int, int);
DECLARE_FAKE_VOID_FUNC(void_function, int, int);
DECLARE_FAKE_VALUE_FUNC_VARARG(int, value_function_vargs, const char *, int, ...);
DECLARE_FAKE_VOID_FUNC_VARARG(void_function_vargs, const char *, int, ...);


/* Private source file file */
#include "public_header.h"

DEFINE_FAKE_VALUE_FUNC(int, value_function, int, int);
DEFINE_FAKE_VOID_FUNC(void_function, int, int);
DEFINE_FAKE_VALUE_FUNC_VARARG(int, value_function_vargs, const char *, int, ...);
DEFINE_FAKE_VOID_FUNC_VARARG(void_function_vargs, const char *, int, ...);

```

## Specifying GCC Function Attributes

You can specify GCC function attributes for your fakes using the `FFF_GCC_FUNCTION_ATTRIBUTES` directive.

### Weak Functions

One usful attribute is the _weak_ attribute that marks a function such that it can be overridden by a non-weak variant at link time.  Using weak functions in combination with fff can help simplify your testing approach.

For example:
* Define a library of fake functions, e.g. libfake.a.
* Link a binary (you might have many) that defines a subset of real variants of the fake functions to the aforementioned fake library.
* This has the benefit of allowing a binary to selectively use a subset of the required fake functions while testing the real variants without the need for many different make targets.

You can mark all fakes with the weak attribute like so:
```
#define FFF_GCC_FUNCTION_ATTRIBUTES __attribute__((weak))
#include "fff.h"
```

See the example project that demonstrates the above approach: _./examples/weak_linking_.

## Find Out More

Look under the examples directory for full length examples in both C and C++.
There is also a test suite for the framework under the test directory.

-------------------------

## Benefits
So whats the point?

 * To make it easy to create fake functions for testing C code.
 * It is simple - just include a header file and you are good to go.
 * To work in both C and C++ test environments


## Under the Hood
 * The fff.h header file is generated by a ruby script
 * There are tests under _./test_
 * There is an example for testing an embedded UI and a hardware driver under _./examples_
 * There is an example of weak_linking under _./examples_


## Cheat Sheet
| Macro | Description | Example |
|-------|-------------|---------|
| FAKE_VOID_FUNC(fn [,arg_types*]); | Define a fake function named fn returning void with n arguments | FAKE_VOID_FUNC(DISPLAY_output_message, const char*); |
| FAKE_VALUE_FUNC(return_type, fn [,arg_types*]); | Define a fake function returning a value with type return_type taking n arguments | FAKE_VALUE_FUNC(int, DISPLAY_get_line_insert_index); |
| FAKE_VOID_FUNC_VARARG(fn [,arg_types*], ...); | Define a fake variadic function returning void with type return_type taking n arguments and n variadic arguments | FAKE_VOID_FUNC_VARARG(fn, const char*, ...) |
| FAKE_VALUE_FUNC_VARARG(return_type, fn [,arg_types*], ...); | Define a fake variadic function returning a value with type return_type taking n arguments and n variadic arguments | FAKE_VALUE_FUNC_VARARG(int, fprintf, FILE*, const char*, ...) |
| RESET_FAKE(fn); | Reset the state of fake function called fn | RESET_FAKE(DISPLAY_init); |
