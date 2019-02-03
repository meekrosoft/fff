#include "test_common.h"
#include "display.h"

DEFINE_FFF_GLOBALS;


int main(void)
{
   init_tests();  // Resets common and hook errors to asserts.

   GLOBAL_TEST_bus_read_ret[2] = 0x10;
   assert( display_init() == true );
   assert( bus_read_write_fake.call_count == 1);

   display_update( "TEST INFO" );
   assert( bus_read_write_fake.call_count == 1 );
   assert( bus_write_fake.call_count == 1 );

   GLOBAL_TEST_bus_read_ret[2] = 0x00;
   assert( display_init() == false );

   printf("Test " __FILE__ " ok\n");
   return 0;
}
