//#include "boost/core/demangle.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

int main() {  
  std::srand(static_cast<unsigned int>(std::time(0)));  
  //boost::optional<int> i = Generator::get_even_random_number();
  int i = 1;
  if (i) {
    std::cout << std::sqrt(static_cast<float>(i)) << "\n";
    //std::cout << boost::core::demangle(typeid(boost::optional<int>).name() << "\n";
  }
}
