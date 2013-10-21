#include <boost/any.hpp>
#include <boost/variant.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <map>

template <typename T>
struct function_traits
{
  typedef typename function_traits<decltype(&T::operator())>::return_type return_type;
  typedef typename function_traits<decltype(&T::operator())>::argument_type argument_type;
};

template <typename R, typename C, typename A>
struct function_traits<R (C::*)(A)> 
{
  typedef R return_type;
  typedef A argument_type;
};

template <typename R, typename C, typename A>
struct function_traits<R (C::*)(A) const> 
{
  typedef R return_type;
  typedef A argument_type;
};

template <typename R, typename A>
struct function_traits<R (*)(A)>
{
  typedef R return_type;
  typedef A argument_type;
};

template <class... T>
void NoOp(T...) { }

template <class... Fs>
struct overload_set : Fs...
{
  overload_set(Fs... f) 
    : Fs(std::move(f))... 
  {}

  void operator ()(boost::any &a) const
  {
    NoOp(try_cast<Fs>(a)...);
  }

  template <class... U>
  void operator ()(boost::variant<U...> &var) const
  {
    NoOp(try_get<Fs>(var)...);
  }
private:

  template <class T, class Variant>
  bool try_get(Variant & var) const
  {
    typedef typename function_traits<T>::argument_type cast_type;
    
    if(boost::get<cast_type>(&var))
      (*this)(*boost::get<cast_type>(&var)); 

    return true;
  }

  template <class T>
  bool try_cast(boost::any & a) const 
  {
    typedef typename function_traits<T>::argument_type cast_type;
    
    if(typeid(cast_type) == a.type())
      (*this)(boost::any_cast<cast_type>(a)); 

    return true;
  }
};

template <class... Fs>
overload_set<Fs...> match(Fs&&... x)
{
  return overload_set<Fs...>(std::forward<Fs>(x)...);
}

void test_any(void)
{
  std::vector<boost::any> va;
  va.push_back(10);
  va.push_back('Z');
  va.push_back(std::string("Sumant"));
  va.push_back(10.20);
  
  for(auto & any : va)
  {
    match(
      [](int i)  { std::cout << "int = " << i << "\n"; },
      [](double d)  { std::cout << "double = " << d << "\n"; },
      [](const std::string & s)  { std::cout << "std::string = " << s << "\n"; },
      [](char c) { std::cout << "char = " << c << "\n"; }
    )(any);
  }
}

void test_variant()
{
  std::vector<boost::variant<int, char, std::string, double>> vv;
  vv.push_back(10);
  vv.push_back('Z');
  vv.push_back(std::string("Sumant"));
  vv.push_back(10.20);
  
  for(auto & var : vv)
  {
    match(
      [](int i)  { std::cout << "int = " << i << "\n"; },
      [](double d)  { std::cout << "double = " << d << "\n"; },
      [](const std::string & s)  { std::cout << "std::string = " << s << "\n"; },
      [](char c) { std::cout << "char = " << c << "\n"; }
    )(var);
  }
}

int main(void)
{
  test_any();
  std::cout << "*********\n";
  test_variant();
}
