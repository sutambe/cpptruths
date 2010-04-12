#include <iostream>
#include <typeinfo>
#include <boost/type_traits.hpp>
#include <boost/function.hpp>

class Base
{
  public:
    int foo() { return 5; }
};

class Derived : public Base
{
};

template <typename T, typename U>
bool IsSame (T t, U u)
{
  return boost::is_same<T,U>::value;
}

template <typename RETURN, typename CLASS, typename ARG>
const char * memfun_class_name (RETURN (CLASS::*func)(ARG))
{
  return typeid(CLASS).name();
}

template <typename RETURN, typename CLASS, typename ARG>
const char * memfun_class_name (RETURN (CLASS::*func)(ARG) const)
{
  return typeid(CLASS).name();
}

template <typename RETURN, typename CLASS>
const char * memfun_class_name (RETURN (CLASS::*func)())
{
  return typeid(CLASS).name();
}

template <typename RETURN, typename CLASS>
const char * memfun_class_name (RETURN (CLASS::*func)() const)
{
  return typeid(CLASS).name();
}

int bar (Base *) { return 500; }
int zoo (int *) { return 500; }

template <class T>
struct MyFunction;

template <class Result, class Para>
struct MyFunction <Result (Para)>
{
  typedef Result Func(Para);
  typedef typename boost::remove_pointer<Para>::type ParaClass;
  typedef Result (ParaClass::*F2)();
  Func *func_;
  F2 f2_;
  MyFunction(Func *f) : func_(f), f2_(0) {}
  MyFunction(F2 f) : func_(0), f2_(f) {}
  Result operator ()(Para p) { return func_ ? func_(p) : (p->*f2_)(); }
};

int main (void)
{
  std::cout << typeid(&Derived::foo).name() << std::endl;
  std::cout << typeid(&Base::foo).name() << std::endl;
  std::cout << IsSame(&Derived::foo, &Base::foo) << std::endl;
  std::cout << memfun_class_name(&Derived::foo) << std::endl;
  std::cout << memfun_class_name(&Base::foo) << std::endl;

  boost::function<int (Base *)> f(&Base::foo);
  boost::function<int (Base *)> b(bar);
  
  MyFunction<int (Base *)> mf(&Base::foo);
  MyFunction<int (Base *)> mb(bar);
  //MyFunction<int (int *)> mb(bar); // Does not work.

  Derived d;
  std::cout << mf(&d);  
  std::cout << mb(&d);  

  typedef int F1(Base *);
  F1 *f1 = bar;
  typedef int (Base::*F2)();
  F2 f2 = &Base::foo;

  return 0;
}
