#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <iterator>

namespace {
  std::string toupper_string (std::string str)
  {
    std::transform (str.begin(), str.end(), str.begin(), ::toupper);
    return str;
  }
}

int main (void)
{
  std::ifstream file ("transform_toupper.cpp");
  std::transform (std::istream_iterator <std::string> (file),
                  std::istream_iterator <std::string> (),
                  std::ostream_iterator <std::string> (std::cout, " "),
                  toupper_string);

  return 0;
}

