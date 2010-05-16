#include <cstdlib>
#include <iostream>

template <typename T>
struct function_traits : function_traits<decltype(&T::operator())> { };

template <typename R, typename C>
struct function_traits<R (C::*)() const>  {
  typedef R result_type;
};

template <typename R, typename C>
struct function_traits<R (C::*)()>  {
  typedef R result_type;
};

void foo(void (*v)(void))
{
  std::cout << "void (*v) (void)\n";
}

template <class R, class T>
void foo(R (T::*)(void))
{
  std::cout << "R (T::*) void)\n";
}
/*
template <class T>
void foo(T)
{
  std::cout << "T\n";
}
*/
int main(void) {
  int counter = 0;
  auto lambda = []{};
  //auto lambda = [&]() mutable {}; // converts to void (*v)()
  //auto lambda = [&]()mutable{ ++counter; }; // Can't convert to void (*v)()
  //auto lambda = [=]{};
  //auto lambda = [=]()mutable{};
  function_traits<decltype(lambda)>::result_type *r;
  foo(lambda);
}


