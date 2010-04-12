#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

void function(std::vector<std::string> vec)
{
}

void function(std::initializer_list<std::string> && ilist)
{
  std::copy(ilist.begin(), ilist.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
}

int main()
{
  std::vector<std::string> vec {"hello", "world", "test"};
  function( {"hello", "world", "test"} );
}

