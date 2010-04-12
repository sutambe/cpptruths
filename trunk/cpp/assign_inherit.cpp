#include <iostream>

struct A
{
  A & operator = (const A &)
  {
    std::cout << "A::operator =\n";
    return *this;
  }
};
struct B : A
{
 /* 
  B & operator = (const B &b)
  {
    A::operator = (b);
    std::cout << "B::operator =\n";
    return *this;
  }
  */
};

int main (void)
{
  B b1, b2;
  b1 = b2;
  return 0;
};

