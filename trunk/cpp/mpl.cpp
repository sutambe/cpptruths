#include <boost/mpl/vector.hpp>
#include <boost/mpl/size.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace boost::mpl;

typedef vector <int, short, long, double, float> 
  Numerical;

int main (void)
{
  std::cout << size<Numerical>::type::value << std::endl;
  //std::cout << Numerical() << std::endl;
  std::vector<Numerical> nv(10);
  std::cout << "Size of mpl::vector = " << sizeof(Numerical) << std::endl;
  return 0;
}

