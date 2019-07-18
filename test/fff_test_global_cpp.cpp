extern "C"{
    #include "global_fakes.h"
}
#include <gtest/gtest.h>

DEFINE_FFF_GLOBALS;

#include "test_cases_assertions_setup.include"

class FFFTestSuite: public testing::Test
{
public:
    void SetUp()
    {
        RESET_FAKE(voidfunc1);
        RESET_FAKE(voidfunc2);
        RESET_FAKE(longfunc0);
        RESET_FAKE(voidfunc1outparam);
        RESET_FAKE(voidfunc3var);
        RESET_FAKE(valuefunc3var);
        FFF_RESET_HISTORY();
        RESET_FAKE_ASSERTIONS();
    }
};

#include "test_cases.include"
#include "test_cases_assertions.include"
