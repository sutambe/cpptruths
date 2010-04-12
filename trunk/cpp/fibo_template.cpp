#include <iostream>

namespace fib_generator {
  template <unsigned long const N>
    struct fibonacci {
      static unsigned long const value =
        fibonacci<N-2>::value
        + fibonacci<N-1>::value ;
    };

  template <>
    struct fibonacci<1> {
      static unsigned long const value = 1uL;
    };

  template <>
    struct fibonacci<2> {
      static unsigned long const value = 1uL;
    };
}

int main(int argc, char * argv[]) {
  using namespace fib_generator;
  std::cout << "20th fibonacci number: " 
    << fibonacci<20uL>::value 
    << std::endl;
  return 0;
};
