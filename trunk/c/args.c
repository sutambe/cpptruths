#include <stdio.h>

     // Error macro
     #define error(text)     \
         // show line number first\
         fprintf( stderr, "Error(%d):", __LINE__ ),\
         fprintf( stderr, "%s\n", text )

     int main( int argc, const char * const *argv )
        {
        int options = 0;
        // check some args are supplied.
        if( argc < 2 ) { error( "args" ); return 1; }
        for( int i = 1; i < argc; i++ )
            if( *argv[i] == '-' ) options++;
        printf( "%d option(s) were supplied to this program\n", options );
		
		// this is a comment continues on next line    \
		printf("Not printed\n");
		printf("printed\n");
        return 0;
        }
