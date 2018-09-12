#ifndef FOO_H__
#define FOO_H__

namespace test {

struct Bar {
  template <class T>
  static std::vector<T> v;
};

template <class T>
std::vector<T> Bar::v{};

struct Foo {
  virtual ~Foo() = default;
};

}

extern "C" void foo(const std::type_info &);
 
#endif  // FOO_H
