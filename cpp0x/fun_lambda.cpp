#include <iostream>
#include <string>
#include <memory>
#include <tuple>
#include <map>
#include <functional>
#include <stdexcept>
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
        auto STRING_JOIN2(scope_exit_, __LINE__) = MakeScopeExit([&](){code;})

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

// Converting frommm boost to std shared_ptr and vice-versa.

template<typename T>
boost::shared_ptr<T> make_shared_ptr(std::shared_ptr<T> ptr)
{
      return boost::shared_ptr<T>(ptr.get(), [ptr](T*) mutable { ptr.reset(); });
}

template<typename T>
std::shared_ptr<T> make_shared_ptr(boost::shared_ptr<T> ptr)
{
      return std::shared_ptr<T>(ptr.get(), [ptr](T*) mutable { ptr.reset(); });
}

void fun2()
{
  boost::shared_ptr<int> i_ptr = make_shared_ptr(std::make_shared<int>(10));    
}

void no_op(...) { }

template <class... T>
void foreach(T... args) {
  no_op([=](){ std::cout << args << "\n"; return true; }()...); 
}

void fun3()
{
  foreach(10, 20.2, true);
}

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

void fun8()
{
  //auto identity = [](auto x) { return x; };
  auto add = [](auto x, auto y) { return x+y; };
  auto sub = [](auto x, auto y) { return x-y; };
  auto mul = [](auto x, auto y) { return x*y; };

  // Write a function that takes an argument and 
  // returns a function that returns that argument.
  // Example: idf(5)();
  auto idf = [](auto x) { 
    return [=]() { return x; };
  };
  std::cout << idf(5)() << std::endl;

  // Write a function that adds from two invocations
  // Example: addf(5)(4);
  auto addf = [](auto x) {
    return [=](auto y) {
      return x + y;
    };
  };
  std::cout << addf(5)(4) << std::endl;

  // Write a function that takes a binary function 
  // and makes it callable with two invocations.
  // Example: applyf(mul)(3)(4)
  auto applyf = [](auto binary) {
    // I find this need to capture the variables quite annoying 
    // Not required in C# for instance.
    return [=](auto x) {
      return [=](auto y) {
        return binary(x, y);
      };
    };
  };
  std::cout << applyf(mul)(3)(4) << std::endl;

  // Write a function that takes a function and 
  // an argument and returns a function that takes
  // t second argument and applies the function.
  // Example: curry(add, 10)(20)
  // schönfinkelisation!
  auto curry = [](auto binary, auto x) {
    return [=](auto y) {
      return binary(x, y);
    };
  };
  std::cout << curry(add, 10)(20) << std::endl;
  
  auto curry2 = [applyf](auto binary, auto x) {
    return applyf(binary)(x);
  };
  std::cout << curry2(sub, 20)(10) << std::endl;

  // Without creating a new function show 3 ways
  // to create the inc function
  // Example: inc(inc(5)) == 7
  auto inc1 = addf(1);
  auto inc2 = applyf(add)(1);
  auto inc3 = curry(add, 1);

  std::cout << inc1(5) << " " << inc2(5) << " " << inc3(5) << std::endl;

  // Write a function twice that takes a binary function
  // and returns a unary function that passes its argument
  // to the binary function twice.
  // Example: twice(mul)(11) == 121
  auto twice = [](auto binary) {
    return [=](auto a) {
      return binary(a, a);
    };
  };
  std::cout << twice(mul)(11) << std::endl;

  // Write a function that swaps the arguments of 
  // a binary function.
  // Example: switcheroo(sub)(3, 2) == -1
  auto switcheroo = [](auto binary) {
    return [=] (auto x, auto y) {
      return binary(y, x);
    };
  };
  std::cout << switcheroo(sub)(3,2) << std::endl;

  // Write a function composeu that takes two unary functions
  // and returns a unary function that calls them both
  // Example: composeu(inc1, curry(mul, 5))(3) == 20
  auto composeu = [](auto bin1, auto bin2) {
    return [=](auto x) {
      return bin2(bin1(x));
    };
  };
  std::cout << composeu(inc1, curry(mul,5))(3) << std::endl;

  // Write a function composeb that takes two binary functions
  // and returns a function that calls them both.
  // Example: composeb(add, mul)(2, 3, 5)
  auto composeb = [](auto bin1, auto bin2) {
    return [=](auto x, auto y, auto z) {
      return bin2(bin1(x, y), z);
    };
  };
  std::cout << composeb(add, mul)(2,3,5) << std::endl;

  // Write a once function that allows a binary function to be 
  // called exactly once.
  auto once = [](auto binary) {
    bool done = false;
    return [=](auto x, auto y) mutable {
      if(!done) 
      {
        done = true;
        return binary(x,y);
      }
      else
        throw std::runtime_error("Exactly once!");
    };
  };
  auto once_mul = once(mul);
  std::cout << once_mul(3,4) << std::endl;
  // std::cout << once_mul(3,4) << std::endl; // throws an exception

  // Write a function fromto that produce a function that will product
  // values in a range.
  // Example: fromto(0, 10)

  auto fromto = [](auto begin, auto end) {
    return [=]() mutable {
      if(begin < end)
        return begin++;
      else
        throw std::runtime_error("Already completed");
    };
  };

  auto stream = fromto(0,3);
  // on gcc the following line produces 2 1 0 due to sequence point issues.
  std::cout << stream() << " " << stream() << " " << stream() << std::endl;

  // Write an element function that takes an array and an optional function
  // (like the result of fromTo) and produces a function that will produce the
  // elements of the array
  auto element = [](std::vector<int> v, 
                    auto func) {
    return [=]() mutable {
      return v[func()];
    };
  };
  auto elm = element({ 'A', 'B', 'C'}, fromto(0,3));
  std::cout << elm() << " " << elm() << " " << elm() << std::endl;

  // Write a function that takes a function and a predicate
  // and produces a function that produces valus that
  // satisfy the predicate.

  auto filter = [](auto func, auto predicate) {
    return [=](auto x) {
      if(predicate(x))
        return func(x);
      else
        // No Monad here so must report error.
        throw std::runtime_error("no match!");
    };
  };
  std::cout << filter(curry(add, 1), [](auto v) { return v==2; })(2) << std::endl;

  // Write a function concat that takes two sequence
  // generating functions and appends one another.

  auto concat = [](auto seq1, auto seq2) {
    bool seq1done = false;
    return [=]() mutable {
      if(!seq1done) 
      {
        try {
          return seq1();
        }
        catch(...) {
          seq1done = true;
          return seq2();
        }
      }
      else
        return seq2();
    };
  };

  auto con = concat(fromto(0, 2), fromto(2, 4));
  std::cout << con() << con() << con() << con() << std::endl;

  // Implement fibonacci
  auto fibo = [=](auto a, auto b) {
    return concat(element({a,b}, fromto(0,2)),
                  [=,next=a+b]() mutable {
                    a=b;
                    b=next;
                    auto temp = next;
                    next = a+b;
                    return temp;
                  });
  };
  auto fib = fibo(1,1);
  std::cout << fib() << fib() << fib() << fib() << fib() << fib() << std::endl;

  // Write a function that takes a binary function and returns 
  // a function that takes two arguments and a callback
  auto binaryc = [](auto binary) {
    return [=](auto x, auto y, auto callback) {
      return callback(binary(x,y));
    };
  };

  std::cout << binaryc(mul)(5, 6, inc1) << std::endl;

}


int main(int argc, char * argv[])
{
  //fun1();
  //fun2();
  //fun3();
  //fun4();
  //if(argc >= 2) fun5(atoi(argv[1]));

  //fun6();
  //http://jrb-programming.blogspot.co.uk/2012/11/another-alternative-to-lambda-move.html

  fun8();
}
