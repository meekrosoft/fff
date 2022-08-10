
#include "display.fake.h"
#include "sensor.fake.h"
#include "test_common.h"

DEFINE_FFF_GLOBALS;


int update_main( void );

int main(void)
{
    init_tests();  // Resets common and hook errors to asserts.

    sensor_init_fake.return_val = true;
    display_init_fake.return_val = true;

    update_main();

    assert( sensor_init_fake.call_count == 1 );
    assert( display_init_fake.call_count == 1 );
    assert( display_update_fake.call_count == 1 );

    printf("Test " __FILE__ " ok\n");
    return 0;
}
