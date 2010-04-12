#include <boost/typeof/typeof.hpp>
#include <iostream>

template <int I>
struct X
{
  enum { value = I };
};

int main(void)
{
  X<(16 > 1)> a;                 // Works on vc9, works on g++ 4.4.2, works on Comeau 4.3.10.1
  X<(int(16) > 1)> b;            // Works on vc9, works on g++ 4.4.2, works on Comeau 4.3.10.1
  X<(16 >> 1)> c;                // Works on vc9, works on g++ 4.4.2, works on Comeau 4.3.10.1
  X<(int(16) >> 1)> d;           // Fails on vc9, works on g++ 4.4.2, works on Comeau 4.3.10.1

  //X<16 > 1> e;                   // Fails on vc9, fails on g++ 4.4.2, fails on Comeau 4.3.10.1
  //X<int(16) > 1> f;              // Fails on vc9, fails on g++ 4.4.2, fails on Comeau 4.3.10.1
  X<16 >> 1> g;                  // Fails on vc9, works on g++ 4.4.2, works on Comeau 4.3.10.1 C++0x disabled
  X<int(16) >> 1> h;             // Fails on vc9, works on g++ 4.4.2, works on Comeau 4.3.10.1 C++0x disabled

  return 0;
}

