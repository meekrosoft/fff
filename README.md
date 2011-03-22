# Fake Function Framework  (fff)
-----------------------------
> How long can we _maintain_? I wonder. How long before one of us starts raving 
> and jabbering at this boy? What will he think then? This same lonely desert 
> was the last known home of the Manson family. Will he make that grim 
> connection...

## A Fake Function Framework for C
fff is a micro-framework for creating fake C functions for tests.  Because life
is too short to spend time hand-writing fake functions for testing.


## Hello fake world

Say you are testing an embedded user interface and you have a function that 
you want to create a fake for:

    // UI.c
    ...
    void DISPLAY_init();
    ...

Here's how you would define a fake function for this in your test suite:

    // test.c(pp)
    #include "fff.h"
    FAKE_VOID_FUNC0(DISPLAY_init);

And the unit test might look something like this:

    TEST_F(GreeterTests, init_initialises_display)
    {
	    UI_init();
	    ASSERT_EQ(1, DISPLAY_init_call_count);
    }

So what has happened here?  The first thing to note is that the framework is 
header only, all you need to do to use it is download <tt>fff.h</tt> and include
it in your test suite.  

The magic is in the <tt>FAKE_VOID_FUNC0</tt>.  This 
expands a macro that defines a function returning <tt>void</tt> 
which has zero arguments.  It also defines a variable 
<tt>"function_name"_call_count</tt> which is incremented every time the faked 
function is called.

Under the hood it generates some code that looks like this:

    static int DISPLAY_init_call_count = 0;
    void DISPLAY_init(){
        DISPLAY_init_call_count++;
	}





## Capturing arguments

Ok, enough with the toy examples.  What about faking functions with arguments?

    // UI.c
    ...
    void DISPLAY_output_message(char * message);
    ...

Here's how you would define a fake function for this in your test suite:

    FAKE_VOID_FUNC1(DISPLAY_output_message, const char*);

And the unit test might look something like this:

    TEST_F(GreeterTests, given_name_when_greet_called_outputs_name)
    {
	    char name[] = "mike";
    	UI_greet(name);
    	ASSERT_EQ(1, DISPLAY_output_message_call_count);
    	ASSERT_EQ(name, DISPLAY_output_message_arg0_val);
    }

There is no more magic here, the <tt>FAKE_VOID_FUNC1</tt> works as in the 
previous example.  The "1" defines the number of arguments that the function 
takes, and the macro arguments following the function name defines the argument
type (a char pointer in this example).

A variable is created for every argument in the form 
<tt>"function_name"_argN_val</tt>



## Return values

When you want to define a fake function that returns a value, you should use the
<tt>FAKE_VOID_FUNC</tt> family of macros.  For instance:

    // UI.c
    ...
    int DISPLAY_get_line_capacity();
    int DISPLAY_get_line_insert_index();
    ...

Here's how you would define a fake functions for these in your test suite:

    FAKE_VALUE_FUNC0(int, DISPLAY_get_line_insert_index);
    FAKE_VALUE_FUNC0(int, DISPLAY_get_line_capacity);

And the unit test might look something like this:
    
    TEST_F(GreeterTests, given_non_full_screen_will_not_reset_display)
    {
    	char name[] = "mike";
    	// given
    	DISPLAY_get_line_capacity_return_val = 10;
    	DISPLAY_get_line_insert_index_return_val = 0;
    	// when
    	UI_greet(name);
    	// then
    	ASSERT_EQ(0, DISPLAY_clear_call_count);
    	ASSERT_EQ(1, DISPLAY_output_message_call_count);
    }

Of course you can mix and match these macros to define a value function with 
arguments, for instance to fake:

    double pow(double base, double exponent);
    
you would use a syntax like this:

    FAKE_VALUE_FUNC2(double, pow, double, double);



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
    
This can be a bit of work if you have many fakes, but luckily if you are testing
in a C++ environment there is a shortcut using static initialization 
registration:

    void SetUp()
	{
        RESET_FAKES(); // resets all fakes
    }

## Call history
Say you want to test that a function calls functionA, then functionB, then 
functionA again, how would you do that?  Well <tt>fff</tt> maintains a call 
history so that it is easy to assert these expectations.

Here's how it works:

    FAKE_VOID_FUNC2(voidfunc2, char, char);
    FAKE_VALUE_FUNC0(long, longfunc0);
    
    TEST_F(FFFTestSuite, calls_in_correct_order)
    {
    	longfunc0();
    	voidfunc2();
    	longfunc0();

    	ASSERT_EQ(call_history[0], (void *)longfunc0);
    	ASSERT_EQ(call_history[1], (void *)voidfunc2);
    	ASSERT_EQ(call_history[2], (void *)longfunc0);
    }
    
They are reset by calling <tt>RESET_HISTORY();</tt>


## Default Argument History

The framework will by default store the arguments for the last ten calls made
to a fake function.
    
    TEST_F(FFFTestSuite, when_fake_func_called_then_arguments_captured_in_history)
    {
        voidfunc2('g', 'h');
        voidfunc2('i', 'j');
        ASSERT_EQ('g', voidfunc2_arg0_history[0]);
        ASSERT_EQ('h', voidfunc2_arg1_history[0]);
        ASSERT_EQ('i', voidfunc2_arg0_history[1]);
        ASSERT_EQ('j', voidfunc2_arg1_history[1]);
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
        ASSERT_EQ(1u, voidfunc2_arg_histories_dropped);
    }

The other is to check if the call count is greater than the history size:

    ASSERT(voidfunc2_arg_history_len < voidfunc2_call_count);
    
The argument histories for a fake function are reset when the RESET_FAKE 
function is called

## User Defined Argument History

Coming soon!

## Function Return Value Sequences

Often in testing we would like to test the behaviour of sequence of function call 
events.  One way to do this with fff is to specify a sequence of return values
with for the fake function.  It is probably easier to describe with an example:

    // faking "long longfunc();"
    FAKE_VALUE_FUNC0(long, longfunc0);
    
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

By specifying a return value sequence using the <tt>SET_RETURN_SEQ<tt> macro, 
the fake will return the values given in the parameter array in sequence.  When
the end of the sequence is reached the fake will continue to return the last 
value in the sequence indefinitely.


## Find out more...

Look under the examlples directory for full length examples in both C and C++.
There is also a test suite for the framework under the test directory.

-------------------------

## Benefits
So whats the point?

 * To make it easy to create fake functions for testing C code.
 * To be simple
 * To work in both C and C++ test environments
 
## What can it do? 
 * It maintains a call count for each fake function
 * It captures the arguments sent in each function call
 * It captures a call history
 * It is simple - just include a header file and you are good to go.

 
## Under the hood:
 * The fff.h header file is generated by a ruby script
 * There are tests under src/test
 * There are examples for testing an embedded UI under src/examples

## Still Todo:
 * Capture argument history
 * Function return value sequences
 * Proper mock syntax - setting expectations, etc   

## Cheat Sheet
<table>
    <tr>
        <th>Macro</th>
        <th>Description</th>
        <th>Example</th>
    </tr>
    <tr>
        <td>FAKE_VOID_FUNC{n}(fn [,arg_types*]);</td>
        <td>Define a fake function named fn returning void with n arguments</td>
        <td>FAKE_VOID_FUNC1(DISPLAY_output_message, const char*);</td>
    </tr>
    <tr>
        <td>FAKE_VALUE_FUNC{n}(return_type, fn [,arg_types*]);</td>
        <td>Define a fake function returning a value with type return_type taking n arguments</td>
        <td>FAKE_VALUE_FUNC0(int, DISPLAY_get_line_insert_index);</td>
    </tr>
    <tr>
        <td>RESET_FAKE(fn);</td>
        <td>Reset the state of fake function called fn</td>
        <td>RESET_FAKE(DISPLAY_init);</td>
    </tr>
    <tr>
        <td>RESET_FAKES();</td>
        <td>Reset all defined fakes</td>
        <td>RESET_FAKES();</td>
    </tr>
</table>