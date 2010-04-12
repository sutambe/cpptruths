#include <algorithm>
#include <functional>
#include <iterator>
#include <iostream>

using namespace std;
  
int main( int argc, char** argv) 
{
    /* The program copies all integers from cin to cout that cannot be divided
     * by the integer parameter given to the program.
     * http://www.mpi-inf.mpg.de/~kettner/courses/lib_design_03/notes/stl.html#Adaptors
     */
    if (argc != 2)
    {
      std::cerr << "Usage: " << argv[0] << " <integer>\n";
      exit(1);
    }
    remove_copy_if( istream_iterator<int>(cin), istream_iterator<int>(),
                    ostream_iterator<int>(cout, "\n"),
                    not1( bind2nd( modulus<int>(), atoi( argv[1]))));
    return 0;
}
