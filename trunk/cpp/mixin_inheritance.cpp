/*#include <iostream>

struct B
{
  void foo ()
  {
    std::cout << "B::foo" << std::endl;
  }
};

void foo ()
{
  std::cout << "::foo" << std::endl;
}

template <typename Base>
struct Mixin : public Base
{
  void bar () const { foo (); }
};

template <class Next>
void function ( const Mixin<Next> &arg)
{
  arg.bar();
}

int main (void)
{
  B *b;
  //Mixin <A> derived;
  Mixin <Mixin <B> > derived2;
  function (derived2);
  b = &derived2;

  return 0;
}
*/
#include <iostream>

struct B {
  void foo () {
    printf ("B::foo");
  }
};

void foo () {
  printf("::foo");
}

template <typename Base>
struct Derived : public Base {
  void bar () { foo (); }
};

int main (void) {
  Derived <B> d;
  d.bar();

  return 0;
}

