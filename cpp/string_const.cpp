#include <iostream>
#include <iomanip>

static void foo (char *s) {
  std::cout << "non-const " << std::hex << static_cast <void *>(s) << std::endl;
}
// A C++ idiom: Always declare a const char* accepting function when
// you have a non-const char* accepting function. Because of default
// conversion rule from string literal to char *, the call to
// foo using in-place literal goes completely undetected through 
// the eyes of compiler's type system.

// Interestingly enough, the addresses of all the identical string literals
// is the same, irrespective of whether it is assigned to const or non-const.
// Internally though, they are stored on the const DATA page and modifying 
// them causes a seg-fault.
static void foo (char const *s) {
  std::cout << "const " << std::hex << static_cast <void const *>(s) << std::endl;
}

int main (void)
{
  char * c1 = "Literal String 2";
  char const * c2 = "Literal String 1";
  foo (c1);
  foo (c2);
  foo ("Literal String 1");
  *c1 = 'l'; // This will cause a seg-fault on Linux.
  std::cout << c2 << std::endl;

    
  return 0;
}

