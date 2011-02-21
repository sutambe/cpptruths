#include <iostream>
#include <vector>
#include <functional>
#include <iterator>
#include <algorithm>
#include <cstdlib>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/front.hpp>
#include <boost/function.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/function_types/parameter_types.hpp>

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
  AdaptedAsBinary(T t) : T(t), x(t) {}
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

int main(void)
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
  std::copy_if(vint.begin(), vint.end(), 
               std::ostream_iterator<int>(std::cout, " "), 
               std::bind([] (int x, int y) { return x < y; }, _1, 5)); 
               //std::bind1st(std::less<int>(), 5)); 
               //std::bind1st(std::ptr_fun(less), 5)); 
  std::cout << std::endl;

  //auto bound = std::bind([] (int x, int y) { return x < y; }, _1, 5); 
  auto bound = std::less<int>(std::bind1st([] (int x, int y) { return x < y; }, 10.5)); // Does not compile.
  // auto bound = std::bind1st(BF(), 10.5); // fails at run-time because BF is empty.
  // auto bound = std::bind1st(std::function<double (double, int)>([](double d, int i){ return d+i; }), 10.5);
  AdaptAsUnary([](double d){ return d; });
  
  std::bind1st(AdaptAsBinary([](double d, int i){ return d+i; }), 10.5);
  //std::cout << "bound = " << bound(5) << std::endl;

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

