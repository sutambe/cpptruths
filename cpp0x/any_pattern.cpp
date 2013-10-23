#include <sys/time.h>
#include <boost/any.hpp>
#include <boost/variant.hpp>
#include <type_traits>
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

template <class Lambda>
struct Otherwise;

template <class T>
struct is_otherwise : std::false_type {};

template <class T>
struct is_otherwise<Otherwise<T>> : std::true_type {};

template <class T>
struct is_otherwise<const Otherwise<T>> : std::true_type {};

template <class T>
struct is_any : std::false_type {};

template <>
struct is_any<boost::any> : std::true_type {};

template <>
struct is_any<const boost::any> : std::true_type {};

template <class T>
struct is_variant : std::false_type {};

template <class... U>
struct is_any<boost::variant<U...>> : std::true_type {};

template <class... U>
struct is_any<const boost::variant<U...>> : std::true_type {};

template <class... T>
void NoOp(T...) { }

template <class Lambda>
struct Otherwise : Lambda
{
  Otherwise(Lambda l) : Lambda(l) {}

  template <class Polymorphic>
  void operator ()(Polymorphic&& p) 
  {
    Lambda::operator()(std::forward<Polymorphic>(p));
  }
};

template <class Lambda>
Otherwise<Lambda> otherwise(Lambda l)
{
  return Otherwise<Lambda>(l);
}

template <class... Fs>
struct overload_set : Fs...
{
  overload_set(Fs... f) 
    : Fs(std::move(f))... 
  {}

  template <class Polymorphic>
  void type_switch(Polymorphic &p) const 
  {
    bool match_once = false;
    NoOp(try_cast<Fs>(p, match_once)...);
  }

private:

  template <class Otherwise, class Polymorphic>
  typename std::enable_if<is_otherwise<Otherwise>::value, bool>::type
  try_cast(Polymorphic & p, bool & match_once) const
  {
    if(!match_once) {
      (*this)(p);
      match_once = true;
    }
    return true;
  }

  template <class T, class Base>
  typename std::enable_if<!is_any<Base>::value &&
                          !is_variant<Base>::value &&
                          !is_otherwise<T>::value &&      
                          !is_otherwise<Base>::value, bool>::type 
  try_cast(Base & base, bool & match_once) const
  {
    typedef typename std::remove_reference<typename function_traits<T>::argument_type>::type cast_type;
    
    if(!match_once && (dynamic_cast<cast_type *>(&base) != nullptr)) {
       (*this)(*dynamic_cast<cast_type *>(&base));
       match_once = true;
    }

    return true;
  }

  template <class T, class... U>
  typename std::enable_if<!is_otherwise<T>::value, bool>::type
  try_cast(boost::variant<U...> & var, bool & match_once) const
  {
    typedef typename function_traits<T>::argument_type cast_type;
    
    if(!match_once && (boost::get<cast_type>(&var) != nullptr)) {
      (*this)(boost::get<cast_type>(var)); 
      match_once = true;
    }

    return true;
  }

  template <class T>
  typename std::enable_if<!is_otherwise<T>::value, bool>::type
  try_cast(boost::any & a, bool &match_once) const 
  {
    typedef typename function_traits<T>::argument_type cast_type;
    
    if(!match_once && (typeid(cast_type) == a.type())) {
      (*this)(boost::any_cast<cast_type>(a)); 
      match_once = true;
    }

    return true;
  }
};

template <class... Fs>
overload_set<Fs...> overload(Fs&&... x)
{
  return overload_set<Fs...>(std::forward<Fs>(x)...);
}

template <class T>
class Matcher
{
  T &t_;
public:  
  Matcher(T &t) : t_(t) {}

  template <class... Fs>
  void operator()(Fs&&... f) const 
  {
    overload_set<Fs...>(std::forward<Fs>(f)...).type_switch(t_);
  }
};

template <class T>
Matcher<T> match(T& t)
{
  return Matcher<T>(t);
}

void test_any(void)
{
  std::vector<boost::any> va;
  va.push_back(10);
  va.push_back('Z');
  va.push_back(std::string("Sumant"));
  va.push_back(10.20);
  va.push_back(true);
  
  for(auto & any : va)
  {
    match(any)(
      [](int i)  { std::cout << "int = " << i << "\n"; },
      [](double d)  { std::cout << "double = " << d << "\n"; },
      [](const std::string & s)  { std::cout << "std::string = " << s << "\n"; },
      [](char c) { std::cout << "char = " << c << "\n"; },
      otherwise([](decltype(any) &a) { 
        std::cout << "Otherwise: no match found: "; 
        if(typeid(bool) == a.type())
          std::cout << std::boolalpha << boost::any_cast<bool>(a) << "\n";
        }) 
    );
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
    match(var)(
      [](int i)  { std::cout << "int = " << i << "\n"; },
      [](double d)  { std::cout << "double = " << d << "\n"; },
      [](const std::string & s)  { std::cout << "std::string = " << s << "\n"; },
      //[](char c) { std::cout << "char = " << c << "\n"; },
      otherwise([](decltype(var) &var) {
        std::cout << "Otherwise: no match found\n"; 
        }) 
    );
  }
}

struct Expr { 
  virtual ~Expr() {}
  virtual int exec() const = 0;
};

struct Value : Expr {
  Value(int v) : value(v) {}
  int exec() const override { return value; }
  int value;
};

struct Plus : Expr {
  Plus(const Expr& e1, const Expr& e2) 
    : e1(e1), e2(e2) 
  {}
  int exec() const override {
    return e1.exec() + e2.exec();
  }
  const Expr& e1;
  const Expr& e2;
};

struct Minus : Expr {
  Minus(const Expr& e1, const Expr& e2) : e1(e1), e2(e2) {}
  int exec() const override {
    return e1.exec() - e2.exec();
  }
  const Expr& e1;
  const Expr& e2;
};

struct Times : Expr {
  Times(const Expr& e1, const Expr& e2) : e1(e1), e2(e2) {}
  int exec() const override {
    return e1.exec() * e2.exec();
  }
  const Expr& e1;
  const Expr& e2;
};

struct Divide : Expr {
  Divide(const Expr& e1, const Expr& e2) : e1(e1), e2(e2) {}
  int exec() const override {
    return e1.exec() / e2.exec();
  }
  const Expr& e1;
  const Expr& e2;
};

int eval(const Expr& expr)
{
  int val = 0;
  match(expr)(
    [&](const Value&  x) { val = x.value; },
    [&](const Plus&   x) { val = eval(x.e1) + eval(x.e2); },
    [&](const Minus&  x) { val = eval(x.e1) - eval(x.e2); },
    [&](const Times&  x) { val = eval(x.e1) * eval(x.e2); },
    [&](const Divide& x) { val = eval(x.e1) / eval(x.e2); }
  );
  return val;
}

int operator - (const timeval & end, const timeval & start)
{
  return (end.tv_sec*1000+end.tv_usec/1000)-(start.tv_sec*1000+start.tv_usec/1000);
}

void test_expr(int iter)
{
  timeval start, stop;
  gettimeofday(&start, nullptr);
  for(int i=0;i < iter;++i)
  {
    eval(Times(Plus(Value(10), Value(20)), Plus(Value(5), Value(10))));
    //std::cout << "eval = " 
    //        << eval(Times(Plus(Value(10), Value(20)), Plus(Value(5), Value(10))))
    //        << std::endl;
  }
  gettimeofday(&stop, nullptr);
  std::cout << "Diff = " << stop - start << std::endl;
  
  gettimeofday(&start, nullptr);
  for(int i=0;i < iter;++i)
  {
    Times(Plus(Value(10), Value(20)), Plus(Value(5), Value(10))).exec();
    //std::cout << "eval = " 
    //        << eval(Times(Plus(Value(10), Value(20)), Plus(Value(5), Value(10))))
    //        << std::endl;
  }
  gettimeofday(&stop, nullptr);
  std::cout << "Diff = " << stop - start << std::endl;
}

int main(int argc, char *argv[])
{
  test_any();
  std::cout << "*********\n";
  test_variant();
  std::cout << "*********\n";
  if(argc >= 2)
    test_expr(atoi(argv[1]));
}
