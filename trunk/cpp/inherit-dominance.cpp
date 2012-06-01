/*
Warning on Visual Studio: cl.exe inherit-dominance.cpp /EHsc /W2
No warning on Comeau, gcc 4.7, clang

*/

#include <iostream>

struct base {   
  virtual int number() { return 0; } 
};

struct weak : public virtual base {   
  using base::number;
  void print() { // seems to only depend on base, but depends on dominant
    std::cout << number() << std::endl;   // template method pattern.
  }
};

struct dominant : public virtual base {   
  int number() { return 5; }
};

struct derived : public virtual weak, public virtual dominant 
{
  using weak::number;
  /*int number() {
    return weak::number();
  }*/
};

int main() {   
  weak w; 
  w.print();    // 0   
  derived d; d.print(); // 5
}
