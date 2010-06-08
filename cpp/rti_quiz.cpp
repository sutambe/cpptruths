#include <iostream>

struct A
{
  int a;
  virtual void print() {
    std::cout << "A::a\n";
  }
};

struct B : A
{
  int b;
  virtual void print() {
    std::cout << "B::b\n";
  }
};

int main(void)
{
  A * array = new B[4];

  for(int i = 0;i < 4; ++i)
  {
    array[i].print();
  }
}


