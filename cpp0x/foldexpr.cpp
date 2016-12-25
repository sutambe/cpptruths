#include <functional>
#include <iostream>
#include <type_traits>
#include <vector>
#include <future>

template <class T>
struct Vector : public std::vector<T> {
  using std::vector<T>::vector;
};

template <class Func>
struct is_function : std::false_type {};

template <class Func>
struct is_function<std::function<Func>> : std::true_type {};

struct Identity
{
  template <class T>
  T operator()(T&& t) { return t; }
};

template <class A, class B, class C>
std::function<C(A)> operator >> (std::function<B(A)> f, std::function<C(B)> g)
{
  return [=](A a){ return g(f(a)); };
}

template <class A, class B>
std::function<B(A)> operator >> (Identity, std::function<B(A)> g)
{
  return g;
}

template <class F, class G>
auto operator >> (F&& f, G&& g)
{
  return [f=std::forward<F>(f), 
          g=std::forward<G>(g)]
          (auto a){ return g(f(a)); };
}

template <class Func>
std::enable_if_t<!is_function<Func>::value, Func> 
operator >> (Identity, Func&& f)
{
  return std::forward<Func>(f);
}

template <class... Args>
auto compose(Args&&... args)
{
  return (Identity() >> ... >> args);
}


template <class T>
std::future<std::remove_reference_t<T>> make_ready_future(T&& t)
{
 auto fut = std::async(std::launch::async, [t](){ return t; });
 fut.wait();
 return fut;
}

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

template <typename R, typename C, typename... A>
struct result_type<R (C::*)(A...)> {
  using type = R;
};

template <typename R, typename C, typename... A>
struct result_type<R (C::*)(A...) const> {
  using type = R;
};

template <typename R, typename... A>
struct result_type<R (*)(A...)> {
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
struct head 
{
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
std::ostream & operator << (std::ostream & o, const std::vector<T> & vec)
{
  for(const T& i: vec) {
    o << i << " ";   
  }
  return o;
}

template <class T>
std::function<std::future<T>(T)> mrdf = make_ready_future<T>;

int main(void)
{
  auto add3 = [](int a){ return a+3; };
  auto mul4 = [](int a){ return a*4; };
  auto sub10 = [](int a){ return a-10; };

  std::function<int(int)> add3f = add3;;
  std::function<int(int)> mul4f = mul4;
  std::function<int(int)> sub10f = sub10;

  std::cout << (add3 >> mul4 >> sub10)(4)       << "\n";
  std::cout << (add3f >> mul4f >> sub10f)(5)    << "\n";
  std::cout << compose(add3f, mul4f, sub10f)(6) << "\n";
  std::cout << compose(add3, mul4, sub10)(7)    << "\n";
  std::cout << compose()(5)                     << "\n";
  
  auto triple = [](int a){ return Vector<int>{3*a,4*a,5*a}; };  
  auto quadruple = [](int a){ return Vector<int>{3*a,4*a,5*a,6*a}; };  
 
  std::cout << (triple >>= quadruple)(1)          << "\n";
  std::cout << composeM(triple, quadruple)(2)     << "\n";

  //std::cout << (make_ready_future<int> >>= mrdf<int>)(5).get() << "\n";
  std::cout << (mrdf<int> >>= mrdf<int>)(5).get() << "\n";
  
  return 0;
}
