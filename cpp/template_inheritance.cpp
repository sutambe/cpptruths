#include <iostream>

template <class T>
class Base 
{
  public:
    Base (int i) {}
};

template <class T>
class Derived : public Base <T>, public Base <double>
{
  public:
    Derived (int i) : Base <T> (i), Base <double> (i)
    {
    }
    
};

int main(void)
{
  Derived <int> d (10);
}

