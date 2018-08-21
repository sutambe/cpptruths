#ifndef FOO_H__
#define FOO_H__

namespace test {

class Foo {
  virtual ~Foo() = default;
};

}
using namespace test;
extern "C" void foo(const std::type_info &);
 
#endif  // FOO_H
