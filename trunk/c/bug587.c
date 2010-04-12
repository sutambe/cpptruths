#include <stdlib.h>
// check_serial returns true if the serial number is OK.
typedef enum { false, true } bool;
static bool check_serial( unsigned serial )
{
    return ((serial & 0x10) == 0x11);
    // This comparison can never be true!
}
// main() returns 0 if the serial number is OK and non-zero otherwise.
int main( int argc, char* argv[] )
{
    unsigned serial_no;
    if( argc <= 1 ) return 2;
    serial_no = (unsigned) atoi( argv[1] );
    return check_serial( serial_no )? 0 : 1;
}

