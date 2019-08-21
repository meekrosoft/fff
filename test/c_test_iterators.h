//VA_ARG based iteration macros for c_test_framework.h

#define RUN_INDEXED_TEST(SUITE, TEST_COUNT, TESTNAME, IDX) do { printf(" Running (%d/%d) %s.%s: \n", IDX+1, TEST_COUNT, #SUITE, #TESTNAME); setup(); TESTNAME(); printf(" SUCCESS\n"); } while (0); \


#define RUN_TESTS(SUITE, ...) PP_2PAR_EACH_IDX(RUN_INDEXED_TEST, SUITE, PP_NARG(__VA_ARGS__), __VA_ARGS__)
