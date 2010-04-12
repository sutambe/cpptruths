#include <iostream>
#include <algorithm>
#include <iterator>
#include <list>

int main (void)
{
  std::list<int> l(4);
  std::fill_n (std::ostream_iterator <int> (std::cout, "\n"),
               5, 10);
  std::fill (l.begin(),l.end(),20);
  std::copy (l.begin(), l.end(),
             std::ostream_iterator <int> (std::cout, "\n"));
  return 0;
}
