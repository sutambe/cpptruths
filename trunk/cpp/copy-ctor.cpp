#include <string>
#include <iostream>

struct X 
{
  X const & self_;
  X(X const & x) : self_(x) 
  {
    std::cout << this << "\n" << &self_ << std::endl;
  } 
};

int main(void)
{
  //std::string s(s); // Crashes.
  X x(x); // Prints same pointer values.
}
