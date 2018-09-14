// This file is a "Hello, world!" in C++ language by GCC for wandbox.
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <string>
#include <numeric>
#include <functional>
#include <iomanip>

int answer;
decltype(auto) ask() { return (answer); }
typedef decltype(ask)* function_ptr;
void func(int &) { }
class convertible {
public:
  operator function_ptr() const {
    return ask;
  }
  operator int &() const {
    return answer;
  }
};

class older_than {
  int limit;
};
void func(older_than) {

std::function<bool(std::string)> e1 = &std::string::empty;
std::function<bool(std::string &)> e2 = &std::string::empty;
std::function<bool(const std::string &)> e3 = &std::string::empty;
std::function<bool(std::string *)> e4 = &std::string::empty;
std::function<bool(const std::string *)> e5 = &std::string::empty;

std::function<void(std::string, char)> p1 = &std::string::push_back;
std::function<void(std::string &, char)> p2 = &std::string::push_back;
std::function<void(std::string *, char)> p4 = &std::string::push_back;

// std::function<void(const std::string &, char)> p3 = &std::string::push_back;
// std::function<void(const std::string *, char)> p5 = &std::string::push_back;

}

struct String : public std::string {
  String(const char *s) : std::string(s) {}
  void push_back(char c) {
    std::string::push_back(c);
  }
  bool empty() const {
    return std::string::empty();
  }
};

struct Arg1 {} _1;
struct Arg2 {} _2;

template <class Func, class A1, class A2>
decltype(auto) bind_helper(Func f, A1 a1, A2 a2) {
  return f(a1,a2);
}

template <class Func>
decltype(auto) bind_helper(Func f, Arg2, Arg1) {
  return [f](auto first_arg, auto second_arg) {
    return f(second_arg, first_arg);
  };
}

template <class Func>
decltype(auto) bind_helper(Func f, Arg1, Arg2) {
  return [f](auto first_arg, auto second_arg) {
    return f(first_arg, second_arg);
  };
}

template <class Func, class A2>
decltype(auto) bind_helper(Func f, Arg1, A2 a2) {
  return [f, a2](auto first_arg) {
    return f(first_arg, a2);
  };
}

template <class Func, class A1>
decltype(auto) bind_helper(Func f, A1 a1, Arg1) {
  return [f,a1](auto second_arg) {
    return f(a1, second_arg);
  };
}

template <class Func, class A1, class A2>
decltype(auto) bind(Func&& f, A1&& a1, A2&&a2) {
  return bind_helper(std::forward<Func>(f), std::forward<A1>(a1), std::forward<A2>(a2));
}

int main()
{
  using F = int&();
  F *f = ask;
  f();
  convertible c;
  c();
  //c++;
  func(c);
  std::function<bool(String)> f2 = &String::empty;
  std::function<bool(String &)> f6 = &String::empty;
  std::function<bool(const String &)> f7 = &String::empty;
  std::function<bool(String *)> f8 = &String::empty;
  std::function<bool(const String *)> f9 = &String::empty;

  std::function<void(std::string &, char)> f3 = &std::string::push_back;
  std::function<void(String &, char)> f4 = &String::push_back;
  f4 = f3;
  std::function<void(String, char)> f5 = &std::string::push_back;
  using F4 = decltype(&std::string::empty);
  F4 f11 = &std::string::empty;
  using F5 = decltype(&std::string::push_back);
 
  String abcd = "abcd";
  std::cout << std::boolalpha << f2(abcd) << " " << f6(abcd) << " " << f7(abcd) << "\n";
  std::cout << std::boolalpha << f8(&abcd) << " " << f9(&abcd) << "\n";
  std::cout << std::boolalpha << bind(std::greater<int>(), _1, 42)(43) << "\n";
  std::cout << std::boolalpha << bind(std::greater<int>(), 42, _1)(43) << "\n";
  std::cout << std::boolalpha << bind(std::greater<int>(), _1, _2)(43, 42) << "\n";
  std::cout << std::boolalpha << bind(std::greater<int>(), _2, _1)(43, 42) << "\n";
  //std::cout << std::boolalpha << std::bind(std::greater<int>(), 42, _2)(43) << "\n";
  std::cout << std::boolalpha << std::bind(std::greater<int>(), 42, 43)() << "\n";

}