#include <iostream>

class base
{
  protected:
    int p;
};

class derived: public base
{
  public:
    void func (base &b)
    {
      std::cout << base::p <<  std::endl;
      std::cout << b.p   <<  std::endl;
    }
};

int main (void)
{
  return 0;
}


