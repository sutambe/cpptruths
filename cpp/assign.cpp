#include <iostream>

struct Base 
{
public:
  virtual Base & operator = (const Base &) = 0;
};

struct Derived : public Base
{
  Derived & operator = (const Derived &)
  {
    std::cout << "Derived::=\n";
    return *this;
  }

};

  Base & Base::operator = (const Base &) 
  {
    std::cout << "Base::=" << std::endl;
    return *this;
  }
int main(void)
{
  Derived d1,d2;
  d1 = d2;

  return 0;
}

