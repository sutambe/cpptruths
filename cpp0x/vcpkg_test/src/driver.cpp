#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <typeinfo>

#include "boost/core/demangle.hpp"
#include "boost/filesystem.hpp"
#include "driver.h"

void check_exists(const char *filename) {
  using namespace boost::filesystem;
  path p(filename);

  if (exists(p)) {   // does p actually exist?
	  if (is_regular_file(p))        // is p a regular file?
		  std::cout << p << " size is " << file_size(p) << '\n';
	  else if (is_directory(p))      // is p a directory?
  		std::cout << p << " is a directory\n";
  	else
	  	std::cout << p << " exists, but is neither a regular file nor a directory\n";
  }
  else
	  std::cout << p << " does not exist\n";
}

int main() {  
  std::srand(static_cast<unsigned int>(std::time(0)));  
  boost::optional<int> i = Generator::get_even_random_number();
  if (i) {
    std::cout << std::sqrt(static_cast<float>(*i)) << "\n";
    std::cout << boost::core::demangle(typeid(boost::optional<int>).name()) << "\n";
  }
  check_exists("driver");
}
