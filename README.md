# Fake Function Framework  (fff)
-----------------------------
> How long can we _maintain_? I wonder. How long before one of us starts raving 
> and jabbering at this boy? What will he think then? This same lonely desert 
> was the last known home of the Manson family. Will he make that grim 
> connection...

## A Fake Function Framework for C
fff is a micro-framework for creating fake C functions for tests.  Because life
is too short to spend time hand-writing fake functions for testing.


## Hello fake world!

Say you are testing an embedded user interface and you have a function that 
you want to create a fake for:

    // UI.c
    ...
    void DISPLAY_init();
    ...

Here's how you would define a fake function for this in your test suite:

    // test.c(pp)
    #include "fff.h"
    DEFINE_FFF_GLOBALS;
    FAKE_VOID_FUNC(DISPLAY_init);

And the unit test might look something like this:

    TEST_F(GreeterTests, init_initialises_display)
    {
	    UI_init();
	    ASSERT_EQ(DISPLAY_init_fake.call_count, 1);
    }

So what has happened here?  The first thing to note is that the framework is 
header only, all you need to do to use it is download <tt>fff.h</tt> and include
it in your test suite.  

The magic is in the <tt>FAKE_VOID_FUNC</tt>.  This 
expands a macro that defines a function returning <tt>void</tt> 
which has zero arguments.  It also defines a struct 
<tt>"function_name"_fake</tt> which contains all the information about the fake.
For instance, <tt>DISPLAY_init_fake.call_count</tt>is incremented every time the faked 
function is called.

Under the hood it generates a struct that looks like this:

    typedef struct DISPLAY_init_Fake { 
        unsigned int call_count; 
        unsigned int arg_history_len;
        unsigned int arg_histories_dropped; 
        void(*custom_fake)(); 
    } DISPLAY_init_Fake;
    DISPLAY_init_Fake DISPLAY_init_fake;





## Capturing arguments

Ok, enough with the toy examples.  What about faking functions with arguments?

    // UI.c
    ...
    void DISPLAY_output(char * message);
    ...

Here's how you would define a fake function for this in your test suite:

    FAKE_VOID_FUNC(DISPLAY_output, char *);

And the unit test might look something like this:

    TEST_F(UITests, write_line_outputs_lines_to_display)
    {
        char msg[] = "helloworld";
        UI_write_line(msg);
        ASSERT_EQ(DISPLAY_output_fake.call_count, 1);
        ASSERT_EQ(strncmp(DISPLAY_output_fake.arg0_val, msg, 26), 0);
    }


There is no more magic here, the <tt>FAKE_VOID_FUNC</tt> works as in the 
previous example.  The number of arguments that the function takes is calculated,
 and the macro arguments following the function name defines the argument
type (a char pointer in this example).

A variable is created for every argument in the form 
<tt>"function_name"fake.argN_val</tt>



## Return values

When you want to define a fake function that returns a value, you should use the
<tt>FAKE_VALUE_FUNC</tt> macro.  For instance:

    // UI.c
    ...
    unsigned int DISPLAY_get_line_capacity();
    unsigned int DISPLAY_get_line_insert_index();
    ...

Here's how you would define fake functions for these in your test suite:

    FAKE_VALUE_FUNC(unsigned int, DISPLAY_get_line_capacity);
    FAKE_VALUE_FUNC(unsigned int, DISPLAY_get_line_insert_index);

And the unit test might look something like this:
    
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

Of course you can mix and match these macros to define a value function with 
arguments, for instance to fake:

    double pow(double base, double exponent);
    
you would use a syntax like this:

    FAKE_VALUE_FUNC(double, pow, double, double);



## Resetting a fake

Good tests are isolated tests, so it is important to reset the fakes for each
unit test.  All the fakes have a reset function to reset their arguments and 
call counts.  It is good prectice is to call the reset function for all the 
fakes in the setup function of your test suite.

    void setup()
    {
        // Register resets
        RESET_FAKE(DISPLAY_init);
        RESET_FAKE(DISPLAY_clear);
        RESET_FAKE(DISPLAY_output_message);
        RESET_FAKE(DISPLAY_get_line_capacity);
        RESET_FAKE(DISPLAY_get_line_insert_index);
    }

You might want to define a macro to do this:

``` 
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

## Call history
Say you want to test that a function calls functionA, then functionB, then 
functionA again, how would you do that?  Well <tt>fff</tt> maintains a call 
history so that it is easy to assert these expectations.

Here's how it works:

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
    
They are reset by calling <tt>FFF_RESET_HISTORY();</tt>


## Default Argument History

The framework will by default store the arguments for the last ten calls made
to a fake function.
    
    TEST_F(FFFTestSuite, when_fake_func_called_then_arguments_captured_in_history)
    {
        voidfunc2('g', 'h');
        voidfunc2('i', 'j');
        ASSERT_EQ('g', voidfunc2_fake.arg0_history[0]);
        ASSERT_EQ('h', voidfunc2_fake.arg1_history[0]);
        ASSERT_EQ('i', voidfunc2_fake.arg0_history[1]);
        ASSERT_EQ('j', voidfunc2_fake.arg1_history[1]);
    }

There are two ways to find out if calls have been dropped.  The first is to 
check the dropped histories counter:

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

The other is to check if the call count is greater than the history size:

    ASSERT(voidfunc2_fake.arg_history_len < voidfunc2_fake.call_count);
    
The argument histories for a fake function are reset when the RESET_FAKE 
function is called

## User Defined Argument History

If you wish to control how many calls to capture for argument history you can
override the default by defining it before include the <tt>fff.h</tt> like this:

    // Want to keep the argument history for 13 calls
    #define FFF_ARG_HISTORY_LEN 13
    // Want to keep the call sequence history for 17 function calls
    #define FFF_CALL_HISTORY_LEN 17
    
    #include "../fff.h"


## Function Return Value Sequences

Often in testing we would like to test the behaviour of sequence of function call 
events.  One way to do this with fff is to specify a sequence of return values
with for the fake function.  It is probably easier to describe with an example:

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

By specifying a return value sequence using the <tt>SET_RETURN_SEQ</tt> macro, 
the fake will return the values given in the parameter array in sequence.  When
the end of the sequence is reached the fake will continue to return the last 
value in the sequence indefinitely.

## Custom Return Value Delegate

You can specify your own function to provide the return value for the fake. This
is done by setting the custom_fake member of the fake.  Here's an example:

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

## How do I fake a function that returns a value by reference?
The basic mechanism that FFF provides you in this case is the custom_fake field described in the *Custom Return Value Delegate* example above.
 
You need to create a custom function (e.g. getTime_custom_fake) to produce the output optionally by use of a helper variable (e.g. getTime_custom_now) to retrieve that output from. Then some creativity to tie it all together. The most important part (IMHO) is to keep your test case readable and maintainable. 

In case your project uses a C99 compliant C compiler you can even combine all this in a single unit test function so you can easily oversee all details of the test. See the example below.

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

## How do I fake a function with a function pointer parameter?
Using FFF to stub functions that have function pointer parameter can cause problems when trying to stub them. Presented here is an example how to deal with this situation.

If you need to stub a function that has a function pointer parameter, e.g. something like:

```
/* timer.h */
typedef int timer_handle;
extern int timer_start(timer_handle handle, long delay, void (*cb_function) (int arg), int arg);
```

Then creating a fake like below will horribly fail when trying to compile because the FFF macro will internally expand into an illegal variable ```int (*)(int) arg2_val```.

```
/* The fake, attempt one */
FAKE_VALUE_FUNC(int,
                timer_start,
                timer_handle,
                long,
                void (*) (int argument),
                int);
```

The solution to this problem is to create a bridging type that needs only to be visible in the unit tester. The fake will use that intermediate type. This way the compiler will not complain because the types match.

```
/* Additional type needed to be able to use callback in FFF */
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

```
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

## Find out more...

Look under the examlples directory for full length examples in both C and C++.
There is also a test suite for the framework under the test directory.

-------------------------

## Benefits
So whats the point?

 * To make it easy to create fake functions for testing C code.
 * It is simple - just include a header file and you are good to go.
 * To work in both C and C++ test environments
 
 
## Under the hood:
 * The fff.h header file is generated by a ruby script
 * There are tests under src/test
 * There is an example for testing an embedded UI and a hardware driver under src/examples
  

## Cheat Sheet
<table>
    <tr>
        <th>Macro</th>
        <th>Description</th>
        <th>Example</th>
    </tr>
    <tr>
        <td>FAKE_VOID_FUNC(fn [,arg_types*]);</td>
        <td>Define a fake function named fn returning void with n arguments</td>
        <td>FAKE_VOID_FUNC(DISPLAY_output_message, const char*);</td>
    </tr>
    <tr>
        <td>FAKE_VALUE_FUNC(return_type, fn [,arg_types*]);</td>
        <td>Define a fake function returning a value with type return_type taking n arguments</td>
        <td>FAKE_VALUE_FUNC(int, DISPLAY_get_line_insert_index);</td>
    </tr>
    <tr>
        <td>RESET_FAKE(fn);</td>
        <td>Reset the state of fake function called fn</td>
        <td>RESET_FAKE(DISPLAY_init);</td>
    </tr>
</table>