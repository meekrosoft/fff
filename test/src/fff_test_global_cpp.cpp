
extern "C"{
    #include "global_fakes.h"
}
#include "gtest/gtest.h"

DEFINE_FFF_GLOBALS;

class FFFTestSuite: public testing::Test
{
public:
    void SetUp()
    {
        FFF_RESET_CALLED_FAKES();
        FFF_RESET_HISTORY();
    }
};

#include "test_cases.include"


