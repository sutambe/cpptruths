#include <iostream>
#include <iomanip>
#include <vector>
#include <future>
#include <string>

template <class T>
using Vector = std::vector<T>;

template<template <typename> class M>
struct Monad {
  struct Return;
  struct Bind;
};

template<>
struct Monad<Vector> {
  struct Return
  {
    template <class T>
    Vector<std::remove_reference_t<T>> operator()(T t) const { return {t}; }
  };
    
  struct Bind 
  {
    template <class T, class Func>
    static auto call(Vector<T> vec, Func&& func) 
    { 
      using Outer = decltype(func(std::declval<T>()));
      Outer outer;
      for(T& o: vec) {
        Outer inner = func(o);
        for(auto i: inner) {
          outer.push_back(i);   
        }
      }
      return outer;
    }
  };
};

template<>
struct Monad<std::future>
{
  struct Return 
  {
    template <class T>
    std::future<std::remove_reference_t<T>> operator()(T&& t) const { 
        return make_ready_future(std::forward<T>(t)); 
    }
  };
  
  struct Bind 
  {
    template <class T, class Func>
    static auto call(std::future<T>&& fut, Func&& func) { 
      fut.wait();
      return func(fut.get()); 
    }
  };
};

template <typename Func>
struct result_type {
  using type = typename result_type<decltype(&Func::operator())>::type;
};

template <typename Func>
struct result_type<Func &> {
  using type = typename result_type<decltype(&Func::operator())>::type;
};

template <typename R, typename C, typename A>
struct result_type<R (C::*)(A)> {
  using type = R;
};

template <typename R, typename C, typename A>
struct result_type<R (C::*)(A) const> {
  using type = R;
};

template <typename R, typename A>
struct result_type<R (*)(A)> {
  using type = R;
};

template <class T>
using result_type_t = typename result_type<T>::type;

template <class T>
struct monadic;

template <template <typename> class M, class T>
struct monadic<M<T>>
{
    using Return = typename Monad<M>::Return;
    using Bind = typename Monad<M>::Bind;
};

template <class MT>
using monadic_return_t = typename monadic<MT>::Return;

template <class MT>
using monadic_bind_t = typename monadic<MT>::Bind;

template <class F, class G>
auto operator >>= (F&& f, G&& g)
{
  return [=] (auto a) {
      return monadic_bind_t<result_type_t<G>>::call(f(std::move(a)), std::move(g));
  };
}

template <class Head, class... Tail>
struct head {
    using type = Head;   
};

template <class... Args>
auto composeM(Args&&... args)
{
  using Func = typename head<Args...>::type;
  using Return = monadic_return_t<result_type_t<Func>>;
  return (Return() >>= ... >>= args);
}

template <class T>
std::ostream & operator << (std::ostream & o, std::vector<T> & vec)
{
  for(T& i: vec) {
    o << i << " ";   
  }
  return o;
}

struct Continent { operator const char *() { return "Continent"; } };
struct Country { operator const char *() { return "Country"; } }; 
struct State { operator const char *() { return "State"; } }; 
struct City { operator  const char *() { return "City"; } }; 

auto get_countries = [](Continent const & c) -> Vector<Country> {
  Vector<Country> v;
  v.push_back(Country());
  return v;
};

auto get_states = [](Country const & c) -> Vector<State> {
  Vector<State> v;
  v.push_back(State());
  return v;
};

auto get_cities = [](State const & s) -> Vector<City> {
  Vector<City> v;
  v.push_back(City());
  return v;
};

int main(void)
{
  Continent c;
  auto countries = get_countries(c);
  std::cout << countries << "\n";
}
