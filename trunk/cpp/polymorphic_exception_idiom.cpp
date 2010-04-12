#include <iostream>

struct ExceptionBase 
{ 
  virtual void raise() { throw *this; }
  virtual ~ExceptionBase () {}
};
struct ExceptionDerived : ExceptionBase 
{ 
  virtual void raise() { throw *this; }
};
 
void foo(ExceptionBase& e)
{
   e.raise(); // Uses dynamic type of e while rasing an exception.
}
int main (void)
{
  ExceptionDerived e;
  try {
    foo(e);
  }
  catch (ExceptionDerived& e) {
    // Exception raised in foo now matches this catch.
    std::cout << "ExceptionDerived\n";
  }
  catch (...) {
    // not here anymore!
  }
} 

