#pragma once

#include <boost/optional.hpp>

struct Generator {
  static boost::optional<int> get_even_random_number() {	
  int i = std::rand();
  return (i % 2 == 0) ? i : optional<int>{}	
  }
};
