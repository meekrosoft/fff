#include <assert.h>
#include <stdio.h>

void setup();
#define TEST_F(SUITE, NAME) void NAME()
#define RUN_TEST(TESTNAME) printf(" Running %s: ", #TESTNAME); fflush(0); setup(); TESTNAME(); printf(" SUCCESS\n");


/* Initialializers called for every test */
void setup()
{

}

/* Tests go here */
TEST_F(GreeterTests, hello_world)
{
	assert(1 == 0);
}



int main()
{
	setbuf(stderr, NULL);
	fprintf(stdout, "-------------\n");
	fprintf(stdout, "Running Tests\n");
	fprintf(stdout, "-------------\n\n");
    fflush(0);

	/* Run tests */
    RUN_TEST(hello_world);


    printf("\n-------------\n");
    printf("Complete\n");
	printf("-------------\n\n");

	return 0;
}
