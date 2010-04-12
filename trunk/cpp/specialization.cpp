#include <iostream>

template <typename T>
struct A
{
  typedef T Foo;
};

template <typename T>
struct A <T *>
{
  //typedef double Foo;
};

int main (void)
{
  A <int *>::Foo f;
}

