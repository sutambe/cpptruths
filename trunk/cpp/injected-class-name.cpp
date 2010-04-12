#include <iostream>

namespace N {

template <class T>
  class Foo
  {
    public:
      void bar() {}
  };

} 

class X : N::Foo<int>
{
  public:
    void f1 (void) { Foo::bar(); }
    void f2 (void) { N::Foo::bar(); }
};

int main(void)
{
  X x;
  x.f1();

  return 0;
}

