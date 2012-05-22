
#include "global_fakes.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

/* Test Framework :-) */
void setup();
#define TEST_F(SUITE, NAME) void NAME()
#define RUN_TEST(SUITE, TESTNAME) printf(" Running %s.%s: \n", #SUITE, #TESTNAME); setup(); TESTNAME(); printf(" SUCCESS\n");
#define ASSERT_EQ(A, B) assert((A) == (B))
#define ASSERT(A) assert((A))


// DECLARE_GLOBAL_FAKE_VOID_FUNC(global_void_func, int);

// Let's see if I can define a fake here
#define MY_FACE_VOID_FUNC0(FUNCNAME) \
        DECLARE_FAKE_VOID_FUNC0(FUNCNAME) \
        DEFINE_FAKE_VOID_FUNC0(FUNCNAME)

MY_FACE_VOID_FUNC0(test_func);

void setup()
{
    RESET_FAKE(global_void_func);
    RESET_FAKE(test_func);
    RESET_HISTORY();
}


TEST_F(FFFGlobalTestSuite, when_global_void_func_never_called_then_callcount_is_zero)
{
    ASSERT_EQ(global_void_func_fake.call_count, 0);
}

TEST_F(FFFGlobalTestSuite, when_global_void_func_called_once_then_callcount_is_one)
{
    global_void_func();
    ASSERT_EQ(global_void_func_fake.call_count, 1);
}

TEST_F(FFFTestSuite, when_void_func_called_once_and_reset_then_callcount_is_zero)
{
    global_void_func();
    RESET_FAKE(global_void_func);
    ASSERT_EQ(global_void_func_fake.call_count, 0);
}


int main()
{
    setbuf(stdout, NULL);
    fprintf(stdout, "-------------\n");
    fprintf(stdout, "Running Tests\n");
    fprintf(stdout, "-------------\n\n");
    fflush(0);

    /* Run tests */
    RUN_TEST(FFFGlobalTestSuite, when_global_void_func_never_called_then_callcount_is_zero);
    RUN_TEST(FFFGlobalTestSuite, when_global_void_func_called_once_then_callcount_is_one);

    printf("\n-------------\n");
    printf("Complete\n");
    printf("-------------\n\n");

    return 0;
}
