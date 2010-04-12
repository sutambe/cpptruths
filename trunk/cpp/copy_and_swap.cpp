#include <iostream>

struct X
{
  X () { std::cout << "X::X()\n"; }
  X (X const & x) { std::cout << "X::X(X const &)\n"; } 
  X & operator = (X const &x)
  {
    X tmp(x);
    tmp.swap (*this);
    return *this;
  }
  void swap (X &x) throw () { }
};

struct Y
{
  Y () { std::cout << "Y::Y()\n"; }
  Y (Y const & y) { std::cout << "Y::Y(Y const &)\n"; } 
  Y & operator = (Y y)
  {
    y.swap (*this);
    return *this;
  }
  void swap (Y &y) throw () { }
};

X createX() { return X(); }
Y createY() { return Y(); }

int main (void)
{
  {
    X x;
    std::cout << "X created\n";
    x = createX();
  }
  std::cout << "Now check out Y\n";
  {
    Y y;
    std::cout << "Y created\n";
    y = createY();
  }
}
