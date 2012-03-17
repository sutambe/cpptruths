#include <iostream>
#include <typeinfo>
#include <stdlib.h>
#include <cxxabi.h>

template <class...> struct tuple {};
template <class, class> struct pair {};

template <class Head, class... Tail>
struct product
{
  typedef tuple<pair<Head, Tail>...> type;
};

template <class... Args>
struct strip<tuple<Args..>>
{
  typedef 
};

template <class... Args, class... Tuples>
struct strip<tuple<Args..>, Tuples...>
{
  typedef tuple<Args..., typename strip<Tuples...>::type> type;
};

template<class... Arg1>
struct Cartesian
{
  template<class... Arg2>
  struct Inner {
    typedef tuple<typename product<Arg1, Arg2...>::type...> type;
  };
};

int main(void)
{
  typedef Cartesian<short, char, int>::Inner<bool, float, double>::type CartProduct;

  int s;
  char * str = abi::__cxa_demangle(typeid(CartProduct).name(), 0, 0, &s);
  std::cout << str << std::endl;
  free(str);

  return 0;
}

