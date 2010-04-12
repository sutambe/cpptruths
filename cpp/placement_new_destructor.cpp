#include <iostream>

using namespace std;

struct A {
  A () { cout << "A ctor" << endl; }
  ~A () { cout << "A dtor" << endl; }
  int x;
};

int main (void)
{
  void * buff = new char[sizeof(A)];
  A * a = new (buff) A;
  a->~A();
  delete [] buff;
}
