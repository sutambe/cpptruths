#include <iostream>
#include <typeinfo> 
#include <cassert>
#include <vector>

#include "foo.h"

using namespace test;

void foo(const std::type_info &other)
{
  assert(other == typeid(Foo));
  std::cout << "typeid equality = " << std::boolalpha << (other == typeid(Foo)) << std::endl;
  std::cout << "type_info address other = " << &other << " local = " << &typeid(Foo) << std::endl;

  assert(other.hash_code() == typeid(Foo).hash_code());
  std::cout << "typeid hash_code equality = " << std::boolalpha << (other.hash_code() == typeid(Foo).hash_code()) << std::endl;
  std::cout << "typeid name: module=" << typeid(Foo).name() << ", other=" << other.name() << std::endl;
  Bar::v<Foo>.push_back(Foo{});
}
