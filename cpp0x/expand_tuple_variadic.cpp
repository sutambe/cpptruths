#include <tuple>
#include <functional>

// Recursive case, unpack Nth argument.
template<unsigned int N>
struct Apply_aux
{
  template<typename F, typename T, typename... X>
  //static auto apply(F f, const T& t, X... x) -> decltype(Apply_aux<N-1>::apply(f, t, std::get<N-1>(t), x...))
  static typename F::result_type apply(F f, const T& t, X... x)
  {
    return Apply_aux<N-1>::apply(f, t, std::get<N-1>(t), x...);
  }
};

// Terminal case, call the function with unpacked arguments
template<>
struct Apply_aux<0>
{
  template<typename F, typename T, typename... X>
  //static auto apply(F f, const T&, X... x) -> decltype(f(x...))
  static typename F::result_type apply(F f, const T&, X... x) 
  {
    return f(x...);
  }
};

// Actual apply function, 
template<typename F, typename T>
//auto apply(F f, const T& t) -> decltype(Apply_aux<std::tuple_size<T>::value>::apply(f, t))
typename F::result_type apply(F f, const T& t)
{
  return Apply_aux<std::tuple_size<T>::value>::apply(f, t);
}

// Testing
#include <string>
#include <iostream>

int f(int p1, double p2, std::string p3)
{
  std::cout << "int=" << p1 << ", double=" << p2 << ", string=" << p3 << std::endl;
  return 1;
}

int g(int p1, std::string p2)
{
  std::cout << "int=" << p1 << ", string=" << p2 << std::endl;
  return 2;
}

int main()
{
  std::tuple<int, double, char const*> tup(1, 2.0, "xxx");
  std::function<int (int, double, char const *)> func = &f;
  std::cout << apply(func, tup) << std::endl;

}
