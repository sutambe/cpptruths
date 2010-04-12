#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

int main(void)
{
  std::vector <std::string> input 
    (std::istream_iterator <std::string> (std::cin),
     std::istream_iterator <std::string> ()) ;  

  std::copy (input.rbegin(), input.rend(), 
      std::ostream_iterator <std::string> (std::cout, " "));

  return 0;
}

