#include <iostream>

int foo()
{
  return 10;
}

const int foo_const()
{
  return 10;
}

class X
{
  public:
    int a;
    char b;
    X() : a(0), b(0) {}
    static X create() { return X(); } // returns X by value.
    X setA(int i)  { a = i; return *this; } 
    X setB(char c) { b = c; return *this; }
};

std::ostream & operator << (std::ostream & o, X const & x)
{
  o << x.a << " " << x.b;
  return o;
}

int main (void)
{
  //int & iref = foo();
  const int & iref_const = foo();
  //const int & iref_const = foo_const();

  std::cout << X::create().setA(10).setB('Z') << std::endl;
}
