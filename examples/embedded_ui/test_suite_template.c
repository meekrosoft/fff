#include "../../test/c_test_framework.h"

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
    RUN_TEST(GreeterTests, hello_world);


    printf("\n-------------\n");
    printf("Complete\n");
	printf("-------------\n\n");

	return 0;
}
