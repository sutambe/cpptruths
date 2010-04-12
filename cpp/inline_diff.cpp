#include <iostream>
using namespace std;

static inline int f( ) {
      static int g = 0;
          return g++;
}

int main( ){
      f( );
          cout << f( ) << endl;
              return 0;
}
