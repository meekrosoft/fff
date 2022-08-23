#include "fff_preamble.h"
#include "c_test_framework.h"

FAKE_VOID_FUNC(voidfunc1, int);

void setup()
{
    RESET_FAKE(voidfunc1);
    FFF_RESET_HISTORY();
}

TEST_F(FFFTestSuite, fake_function_with_preamble_is_callable)
{
    // note the test case assumes the preamble is "__wrap_"
    __wrap_voidfunc1(10);
    ASSERT_EQ(voidfunc1_fake.call_count, 1);
}

DEFINE_FFF_GLOBALS;
int main()
{
    setbuf(stdout, NULL);
    fprintf(stdout, "-------------\n");
    fprintf(stdout, "Running Tests\n");
    fprintf(stdout, "-------------\n\n");
    fflush(0);

    /* Run tests */
    RUN_TEST(FFFTestSuite, fake_function_with_preamble_is_callable);

    printf("\n-------------\n");
    printf("Complete\n");
    printf("-------------\n\n");

    return 0;
}
