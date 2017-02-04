#ifndef C_TEST_FRAMEWORK_H_
#define C_TEST_FRAMEWORK_H_

#include <assert.h>
#include <stdio.h>
#include <string.h>

/* Test Framework :-) */
void setup();
#define TEST_F(SUITE, NAME) void NAME()
#define RUN_TEST(SUITE, TESTNAME) do { printf(" Running %s.%s: \n", #SUITE, #TESTNAME); setup(); TESTNAME(); printf(" SUCCESS\n"); } while (0)
#define ASSERT_EQ(A, B) assert((A) == (B))
#define ASSERT_TRUE(A) assert((A))

#endif /* C_TEST_FRAMEWORK_H_ */
