#include <iostream>
#include <string>
#include <memory>
#include <tuple>
#include <map>
#include <functional>
#include "boost/shared_ptr.hpp"
#include <sys/time.h>

template <typename F>
struct ScopeExit {
  ScopeExit(F f) : f(f) {}
  ~ScopeExit() { f(); }
  F f;
};

template <typename F>
ScopeExit<F> MakeScopeExit(F f) {
  return ScopeExit<F>(f);
};

#define STRING_JOIN2(arg1, arg2) DO_STRING_JOIN2(arg1, arg2)
#define DO_STRING_JOIN2(arg1, arg2) arg1 ## arg2
#define SCOPE_EXIT(code) \
        auto STRING_JOIN2(scope_exit_, __LINE__) = MakeScopeExit([=](){code;})

template<typename T>
struct is_callable {
private:
    typedef char(&yes)[1];
    typedef char(&no)[2];

    struct Fallback { void operator()(); };
    struct Derived : T, Fallback { };

    template<typename U, U> struct Check;

    template<typename>
    static yes test(...);

    template<typename C>
    static no test(Check<void (Fallback::*)(), &C::operator()>*);

public:
    static const bool value = sizeof(test<Derived>(0)) == sizeof(yes);
};

void fun1()
{
  auto func = [](std::string prefix, std::string suffix) {
    return [suffix, prefix](std::string str) {
      std::cout << prefix << str << suffix;
    };
  };
  auto say_hi = func("Hi", "!\n");
  say_hi(" Sumant");
  say_hi(" Neil");
}

template<typename T>
boost::shared_ptr<T> make_shared_ptr(const std::shared_ptr<T>& ptr)
{
      return boost::shared_ptr<T>(ptr.get(), [ptr](T*){});
}

template<typename T>
std::shared_ptr<T> make_shared_ptr(const boost::shared_ptr<T>& ptr)
{
      return std::shared_ptr<T>(ptr.get(), [ptr](T*){});
}

void fun2()
{
  boost::shared_ptr<int> i_ptr = make_shared_ptr(std::make_shared<int>(10));    
}
/*
template <class... T>
void no_op(const T&&... args) {   }

template <class... T>
void foreach(T... args) {
  no_op([args](){ std::cout << args << "\n"; return true; }()...); 
}

void fun3()
{
  foreach(10, 20.2, true);
}
*/
int test(int i)
{
  return i*2;
}

template <typename ReturnType, typename... Args>
std::function<ReturnType (Args...)> 
memoize(std::function<ReturnType (Args...)> func)
{
    std::map<std::tuple<Args...>, ReturnType> cache;
    return ([=](Args... args) mutable  {
            std::tuple<Args...> t(args...);
            if (cache.find(t) == cache.end())                
            {
              std::cout << "not found\n";
              cache[t] = func(args...);
            }
            return cache[t];
    });
}

template<typename T>
std::function<typename std::enable_if<std::is_function<T>::value, T>::type> 
make_function(T *t) 
{
  return { t };
}

void fun4()
{
  auto mem_test = memoize(make_function(test));
  mem_test(10);
  mem_test(10);
}

struct Foo
{
  Foo() = default;
  void operator = (int i) { }
  Foo(Foo const &) {
    std::cout << "Foo copy-ctor\n";
  }
  Foo(Foo &&) {
    std::cout << "Foo move-ctor\n";
  }
  ~Foo() {
    std::cout << "Foo destructor\n";
  }
};


template <class T>
class func_wrapper
{
  mutable std::shared_ptr<std::function<T>> func;
  std::weak_ptr<std::function<T>> weak_func;
  func_wrapper *captured;

public:

  func_wrapper() 
    : captured(nullptr)
  {}

  func_wrapper(const func_wrapper &f)
    : func(f.func),
      weak_func(f.weak_func),
      captured(f.captured)
  {
    std::cout << "const copy-ctor\n";
  }

  func_wrapper(func_wrapper &&f)
    : func(std::move(f.func)),
      weak_func(std::move(f.weak_func)),
      captured(std::move(f.captured))
  {
    std::cout << "non-const move-ctor\n";
  }

  func_wrapper(func_wrapper & f)
    : func(f.func),
      weak_func(f.weak_func),
      captured(nullptr)
  {
    std::cout << "non-const copy-ctor\n";
    f.captured = this;
  }

  template <class U> 
  typename std::enable_if<is_callable<typename std::remove_reference<U>::type>::value,
                          const func_wrapper &>::type
  operator = (U&& u)
  {
    std::cout << "universal-assign\n";
    weak_func = func = std::make_shared<std::function<T>>();
    if(captured)
    {
      captured->weak_func = func;
      std::cout << "assigned to captured\n";
    }
    (*func) = std::forward<U>(u);
    return *this;
  }

  template <class... Args>
  auto operator () (Args&&... args) const  
    -> decltype((*func)(std::forward<Args>(args)...)) 
  {
    if(func)
      return (*func)(std::forward<Args>(args)...);
    else 
    {
      func = weak_func.lock();
      SCOPE_EXIT(func.reset());
      auto&& ret = (*func)(std::forward<Args>(args)...);
      return ret;
    }
  }
};

void sink(func_wrapper<int(int)> f) { std::cout << "inside sink\n"; }

auto create() -> const func_wrapper<int(int)>
{
  // recursive lambda
  // variable 'fib1' is uninitialized when used within its own initialization
  Foo foo;
  func_wrapper<int(int)> fib1;
  std::cout << "fib1 created\n";
  fib1 = [=](int n) mutable
  {
    foo = 10;
    return (n <= 2)? 1 : fib1(n-1) + fib1(n-2);
  };
  std::cout << "fib1 assigned\n";

  //sink(fib1);
  //std::cout << "sink done\n";
  {
    // This block of code is only to disable NRVO. 
    // The condition is always false 
    if(fib1(4) < 0) 
    {
      func_wrapper<int (int)> fib2;
      return fib2;
    }
  }

  return fib1;
}

int fibo(int n)
{
  return (n <= 2)? 1 : fibo(n-1) + fibo(n-2);
}

int operator - (const timeval & end, const timeval & start)
{
  return (end.tv_sec*1000+end.tv_usec/1000)-(start.tv_sec*1000+start.tv_usec/1000);
}

void fun5(int iter)
{
  auto const fib1 = create();
  std::cout << "direct " << fib1(6) <<std::endl;
  timeval start, end;

  gettimeofday(&start, 0);
  for(int i=0;i<iter;++i)
    fib1(i); 
  gettimeofday(&end, 0);
  std::cout << "diff = " << end-start << std::endl;
  
  gettimeofday(&start, 0);
  for(int i=0;i<iter;++i)
    fibo(i); 
  gettimeofday(&end, 0);
  std::cout << "diff = " << end-start << std::endl;
}

std::function<int(int)> make() {
  return [=](int n) {
           std::function<int(int)> recurse;
           recurse = [&](int n){ return (n<=2)? 1 : recurse(n-1) + recurse(n-2); }; 
           return recurse(n);
         };
}

void fun6()
{
  int foo = 20;
  std::function<int(int)> f1 = 
    [=](int n) mutable {
           std::function<int(int)> recurse;
           recurse = [=,&recurse](int n) mutable { foo = 10; return (n<=2)? 1 : recurse(n-1) + recurse(n-2); }; 
           return recurse(n);
         };
  std::cout << f1(6) << "foo=" << foo << "\n";

  auto f2 = make();
  std::cout << f2(6) << "\n";
}

void fun7()
{
//http://www.introtorx.com/Content/v1.0.10621.0/12_CombiningSequences.html#CombiningMultipleSequences
// Lambda provides an implementation of a virtual function.
/*
  Observable.Create<long>(o =>
      {
      Console.WriteLine("1st subscribed to");
      return Observable.Timer(TimeSpan.FromMilliseconds(500))
      .Select(i=>1L)
      .Subscribe(o);
      });
*/

}

int main(int argc, char * argv[])
{
  //fun1();
  //fun2();
  //fun3();
  //fun4();
  if(argc >= 2)
    fun5(atoi(argv[1]));

  fun6();
  //http://jrb-programming.blogspot.co.uk/2012/11/another-alternative-to-lambda-move.html
}
