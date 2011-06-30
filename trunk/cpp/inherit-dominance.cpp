/*
Visual Studio:
cl.exe inherit-dominance.cpp /EHsc /W2

*/

#include <iostream>

struct base {   
  virtual int number() { return 0; } 
};

struct weak : public virtual base {   
  void print() { // seems to only depend on base, but depends on dominant
    std::cout << number() << std::endl;   
  }
};

struct dominant : public virtual base {   
  int number() { return 5; }
};

struct derived : public weak, public dominant 
{};

int main() {   
  weak w; 
  w.print();    // 0   
  derived d; d.print(); // 5
}
