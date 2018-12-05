
extern "C"{
    #include "global_fakes.h"
}
#include <gtest/gtest.h>

DEFINE_FFF_GLOBALS;

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
    }
};

#include "test_cases.include"


