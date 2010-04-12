#include <iostream>

class Test
{
  public:
    Test * operator -> ()
    {
      std::cout << "Inside operator ->\n";
      return new Test;
    }
    Test & operator ++ ()
    {
      std::cout << "Inside operator ++\n";
      return *this;
    }
    Test func (void)
    {
      std::cout << "Inside func\n";
      return Test();
    }
};

int main(void)
{
  Test t;
  ++t->func();

  return 0;
}

