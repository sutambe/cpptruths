#ifdef WIN32
#include "stdafx.h"
#endif

#include <iostream>
#include <vector>
#include <functional> // for std::binder1st
#include <iterator>
#include <algorithm>
#include <cstdlib>

#include <boost/mpl/for_each.hpp>
#include <boost/mpl/front.hpp>
#include <boost/function.hpp>   // for boost::function
#include <boost/functional.hpp> // for boost::binder1st
#include <boost/function_types/result_type.hpp>
#include <boost/function_types/parameter_types.hpp>

//#include <boost/bind.hpp>
//#include <boost/bind/placeholders.hpp>
//using namespace boost::placeholders;
using namespace std::placeholders;

struct X
{
  float operator () (float i) { std::cout << "float\n"; return i*2;}
  //float operator () (float i,double d) {std::cout << "float, double\n"; return d*2;}
};

template <typename T>
/* Taking address will not work for functors that have templatized function call operator. */
struct function_traits : function_traits<decltype(&T::operator())>
{
};

template <typename R, typename C>
struct function_traits<R (C::*)()> 
{
  typedef R result_type;
};

template <typename R, typename C, typename Arg>
struct function_traits<R (C::*)(Arg)> 
{
  typedef R result_type;
  typedef Arg argument_type;
};

template <typename R, typename C, typename Arg1, typename Arg2>
struct function_traits<R (C::*)(Arg1, Arg2)> 
{
  typedef R result_type;
  typedef Arg1 first_argument_type;
  typedef Arg2 second_argument_type;
};

template <typename R, typename C>
struct function_traits<R (C::*)() const> 
{
  typedef R result_type;
};

template <typename R, typename C, typename Arg>
struct function_traits<R (C::*)(Arg) const> 
{
  typedef R result_type;
  typedef Arg argument_type;
};

template <typename R, typename C, typename Arg1, typename Arg2>
struct function_traits<R (C::*)(Arg1, Arg2) const> 
{
  typedef R result_type;
  typedef Arg1 first_argument_type;
  typedef Arg2 second_argument_type;
};

/*
template <typename R>
struct function_traits<R (*)()> 
{
  typedef R result_type;
};

template <typename R, typename Arg>
struct function_traits<R (*)(Arg)> 
{
  typedef R result_type;
  typedef Arg argument_type;
};

template <typename R, typename Arg1, typename Arg2>
struct function_traits<R (*)(Arg1, Arg2)> 
{
  typedef R result_type;
  typedef Arg1 first_argument_type;
  typedef Arg2 second_argument_type;
};

template <typename R>
struct function_traits<R ()> 
{
  typedef R result_type;
};

template <typename R, typename Arg>
struct function_traits<R (Arg)> 
{
  typedef R result_type;
  typedef Arg argument_type;
};

template <typename R, typename Arg1, typename Arg2>
struct function_traits<R (Arg1, Arg2)> 
{
  typedef R result_type;
  typedef Arg1 first_argument_type;
  typedef Arg2 second_argument_type;
};
*/


/*
template <typename R, typename... A>
struct function_traits<R (*)(A...)> 
{
  typedef R result_type;
};

template <typename R, typename C, typename... A>
struct function_traits<R (C::*)(A...)> 
{
  typedef R result_type;
};

template <typename R, typename... A>
struct function_traits<R (A...)> 
{
  typedef R result_type;
};
*/

template <typename F>
typename function_traits<F>::result_type
foo(F f) 
{
  return f(10);
}

template <typename F>
typename function_traits<F>::result_type
bar(F f) 
{
  return f(5.0f, 100);
}

template <class T>
void print_type (T)
{
  std::cout << "Type = " << typeid(T).name() << std::endl;
}

struct print_type_functor
{
  template <class T>
  void operator ()(T)
  {
    std::cout << "Type = " << typeid(T).name() << std::endl;    
  }
};


int f(int x) { return x + x;  }
bool less(int x, int y) { return x < y;  }
struct zoo : std::binary_function<int,int,int> {
  int operator ()(int x, int y) { return x+y; }
};
struct zoo_const : std::binary_function<int,int,int> {
  int operator ()(int x, int y) const { return x+y; }
};


template <typename T>
struct AdaptedAsUnary : T, 
  std::unary_function<typename function_traits<T>::result_type,
                      typename function_traits<T>::argument_type>
{
  AdaptedAsUnary(T t) : T(t) {}
};

template <typename T>
AdaptedAsUnary<T>
AdaptAsUnary(T t)
{
  return AdaptedAsUnary<T>(t);
}

template <typename T>
struct AdaptedAsBinary : T,
  std::binary_function<typename function_traits<T>::result_type,
                       typename function_traits<T>::first_argument_type,
                       typename function_traits<T>::second_argument_type>
{
  AdaptedAsBinary(T t) : T(t), x(t) { }
  T x; // copy constructed lambda.
};

template <typename T>
AdaptedAsBinary<T>
AdaptAsBinary(T t)
{
  return AdaptedAsBinary<T>(t);
}

void test_function_traits()
{
  //typedef int F1(void);
  //function_traits<F1>::result_type r1;
  //function_traits<F1>::argument_type arg1;
}

#ifdef WIN32
int _tmain(int argc, _TCHAR* argv[])
#else
int main(void)
#endif
{
  test_function_traits();
  //foo(f);
  foo(X());
  bar([](float f, int l){ return f+l; });
  auto test_lambda = [](float f, int l) { return f+l;}; 
  function_traits<decltype(test_lambda)>::result_type rt;
  typedef boost::function<int (double, float)> BF;
  function_traits<BF>::second_argument_type i;

  std::vector<int> vint;
  std::generate_n(std::back_inserter(vint), 10, []() { return rand() % 10; } );
#ifndef WIN32
  std::copy_if(vint.begin(), vint.end(), 
               std::ostream_iterator<int>(std::cout, " "), 
               std::bind([] (int x, int y) { return x < y; }, _1, 5)); 
               //std::bind([] (int x, int y) { return x < 5; })); 
               //std::bind1st(std::less<int>(), 5)); 
               //std::bind1st(std::ptr_fun(less), 5)); 
#endif
  std::cout << std::endl;
  
  // non-mutable
  auto bound = [] (int x) throw() { return x*x; };
  if(noexcept(bound(std::declval<int>())))
    std::cout << "bound does not throw\n";
  else
    std::cout << "bound throws\n";

  // auto bound = std::bind([] (int x, int y) { return x + y; }, _1, 5); // -VS2010, +g++4.5
  // auto bound = std::less<int>(std::bind1st([] (int x, int y) { return x < y; }, 10.5)); // -VS2010, -g++45
  // auto bound = std::bind1st(BF(), 10.5); // +VS2010. +g++45 Fails at run-time because BF is empty.
  // auto bound = std::bind1st(std::function<double (double, int)>([](double d, int i){ return d+i; }), 10.5); // +VS2010 +g++45
  // zoo_const z; auto bound = boost::bind1st(z, 200); // +VS2010 +g++45
  // zoo_const z; auto bound = std::bind1st(z, 200); // +VS2010 +g++45
  // auto bound = std::bind1st(AdaptAsBinary([](double d, int i) { return d+i; }), 10.5); // +VS2010 +g++45

  // mutable
  // auto bound = [vint] (int x) mutable { return vint[0]=(x*x); };
  // auto bound = std::bind([vint] (int x, int y) mutable { return vint[0]=(x + y); }, _1, 5); // -VS2010 +g++45
  // auto bound = std::less<int>(std::bind1st([vint] (int x, int y) mutable { return vint[0]=(x + y); }, 10.5)); // -VS2010, -g++4.5
  /* auto bound = std::bind1st(std::function<double (double, int)> 
     ([vint](double d, int i) mutable { return vint[0]=d+i; }), 10.5); // +VS2010 +g++45 */
  // zoo z; auto bound = boost::bind1st(z, 200); // -VS2010 -g++45
  // zoo z; auto bound = std::bind1st(z, 200); // -VS2010 -g++45
  // auto bound = std::bind1st(AdaptAsBinary([vint](double d, int i) mutable { return (vint[0]=d+i); }), 10.5); // -VS2010 -g++45

  std::cout << "bound = " << bound(5) << std::endl;

  //boost::unary_traits<decltype(bound)>::argument_type arg;
  //boost::unary_traits<decltype(bound)>::result_type result;


  AdaptAsUnary([](double d){ return d; });
  
  typedef bool LessFuncType(int,int);
  boost::function_types::result_type<LessFuncType>::type flag(false);
  typedef boost::function_types::parameter_types<LessFuncType>::type Paras;
  // typedef boost::function_types::parameter_types<decltype(test_lambda)>::type Paras;
  boost::mpl::for_each<Paras>(print_type_functor());

  std::ptr_fun(atoi);
  //std::ptr_fun<int (*)(int)>([](int x){return x;});

  X x;
  float (X::*fptr)(float) = &X::operator();
  (x.*fptr)(10.4);
}

