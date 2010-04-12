#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <cstdlib>

struct X
{
  int &i_;
  X (int &i) : i_(i) {}
  void operator () (int i) const
  {
    std::cout << i << std::endl;    
  }
};
unsigned int my_rand()
{
  return random() % 50;
}

int main(void)
{
  int a = 100;
  srand(time(0));
  std::vector<int> v(10);
#ifdef __GXX_EXPERIMENTAL_CXX0X_
  std::generate(v.begin(), v.end(), [](){ return random() % 50; });
#else
  std::generate(v.begin(), v.end(), my_rand);
#endif
  std::for_each(v.begin(),v.end(), X(a));

}

