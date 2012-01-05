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
   
  printf("%p %p\n", array, &array->a); 
  // Printed addresses are different by 4 bytes because 
  // the vtable pointer is at the beginning of the object.
  // Location of the vtable pointer is not standardized.
  for(int i = 0;i < 4; ++i)
  {
    array[i].print();
  }
}


