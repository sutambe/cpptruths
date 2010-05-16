#include <iostream>
#include <boost/function.hpp>

namespace detail {

template <typename F> struct lambda_traits;

template <typename R, typename C, typename... A>
struct lambda_traits<R (C::*)(A...)> 
{
  enum { value = 0 };
};

template <typename R, typename C, typename... A>
struct lambda_traits<R (C::*)(A...) const> 
{
  enum { value = 0 };
};

template <typename R, typename... A>
struct lambda_traits<R (*)(A...)> 
{
  enum { value = 1 };
};

} // namespace detail

template <typename F>
struct is_lambda
{
  enum { value = detail::lambda_traits<decltype(&F::operator())>::value };
};

template <typename R, typename C, typename... A>
struct is_lambda<R (C::*)(A...)> 
{
  enum { value = 0 };
};

template <typename R, typename C, typename... A>
struct is_lambda<R (C::*)(A...) const> 
{
  enum { value = 0 };
};

template <typename R, typename... A>
struct is_lambda<R (*)(A...)> 
{
  enum { value = 0 };
};

template <typename R, typename... A>
struct is_lambda<R (A...)> 
{
  enum { value = 0 };
};

template <typename F>
void print_is_lambda(F)
{
  std::cout << "is_lambda = " << is_lambda<F>::value << std::endl;
}

struct X {
  void operator ()(int) {}
};

int foo(int) { return 0; }

template <typename F>
F ret_lambda(F f)
{
  return f;
}

int main(void)
{
  print_is_lambda([](){}); // prints 1
  print_is_lambda(X());    // prints 0
  print_is_lambda(foo);    // prints 0
  auto lambda = [](float f, int l, double d) { return f+l+d; }; 
  print_is_lambda(lambda); // prints 1
  print_is_lambda(ret_lambda([](){ return [](float f){return f*2;}; }())); // prints 1
  print_is_lambda(boost::function<int (int)>(foo)); // prints 0
}

