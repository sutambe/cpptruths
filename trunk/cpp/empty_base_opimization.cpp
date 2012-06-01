#include <cstdio>
#include "boost/compressed_pair.hpp"

class E1 {};
class E2 {};

class regular 
{
  E1 e1;
  E2 e2;
  int data;
};

template <class Base1, class Base2, class Member>
struct BaseOptimization : Base1, Base2 {
     Member m;
     BaseOptimization() {}
     BaseOptimization(Base1 const& b1, Base2 const & b2, Member const& mem) 
       : Base1(b1), Base2(b2), m(mem) { }
     Base1 * first()  { return this; }
     Base2 * second() { return this; }
};

struct Foo {
    BaseOptimization<E1, E2, int> data;
}; 


int main(void)
{
  printf("sizeof(regular) = %d\n", sizeof(regular));
  printf("sizeof(Foo) = %d\n", sizeof(Foo));

  Foo f;
  printf("%p %p\n", f.data.first(), f.data.second());

  boost::compressed_pair<E1, E1> cpair;
  printf("boost compressed_pair %p %p\n", &cpair.first(), &cpair.second());

  return 0;
}
