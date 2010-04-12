#include <iostream>
#include <algorithm>
#include <iterator>
#include <boost/foreach.hpp>
#include <ctime>
#include <cstdlib>

#include "Carrier.h"

unsigned int myrandom()
{  
  return rand() % 100;
}

int main (void)
{
  const int SIZE = 5;
  int array[SIZE] = { 10, 20, 30, 40, 50 };
  std::vector<int> v1(2);
  std::vector<int> v2(3);

  srand(time(0));

  std::generate(v1.begin(), v1.end(), myrandom);
  std::generate(v2.begin(), v2.end(), myrandom);

  Carrier<int> int_carrier(array, array + SIZE);

  int_carrier.push_back(v1);
  int_carrier.push_back(v2);

  std::cout << "Size = " << int_carrier.size() << std::endl;
  std::copy(int_carrier.begin(), int_carrier.end(), std::ostream_iterator<int>(std::cout,"\n"));

  swap(int_carrier, int_carrier);

  std::cout << "Running BOOST_FOREACH\n";
  std::vector<int> vint(int_carrier);
  BOOST_FOREACH(const int & i, vint)
  {
    std::cout << i << std::endl;
  }
}
