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

template <typename R>
struct function_traits<R (*)()> {
  typedef R result_type;
};

void foo(int (*v)(void))
{
  std::cout << "int (*v) void)\n";
}

template <class T>
void foo(int (T::*v)(void))
{
  std::cout << "int (T::*v) void)\n";
}

int main(void) {
  auto lambda = [] ()mutable { return rand(); };
  //auto lambda = [&]{};
  //auto lambda = [&]()mutable{};
  //auto lambda = [=]{};
  //auto lambda = [=]()mutable{};
  function_traits<decltype(lambda)>::result_type *r;
  foo(lambda);
}


