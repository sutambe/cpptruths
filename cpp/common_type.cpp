#include <iostream>

void foo (int)
{
  std::cout << "void foo(int)\n";
}

void foo (double)
{
  std::cout << "void foo(double)\n";
}

struct X 
{
  X (double d = 0.0) {}
};

void foo (X)
{
  std::cout << "void foo(X)\n";
}

int main(void)
{
  int i = 0;
  X x;
  foo(i ? x : 0.0);
}


#ifdef __GXX_EXPRERIMENTAL_CXX0X_

#include <type_traits>

int main(void)
{
  typedef std::common_type<int, float>::type C;
  static_assert(std::is_same<C, float>::value, "Message");
  
  int i = 0;
  typedef decltype (i ? 0 : 0.0) F;
  static_assert(std::is_same<F, double>::value, "Message2");
}

#endif
