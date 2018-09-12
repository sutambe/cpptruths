#include <typeinfo>
#include <iostream>
#include <string>
#include <unistd.h>
#include <dlfcn.h>
#include <cassert>
#include <vector>

#include "foo.h"

using namespace test;

void test_typeinfo() {
  char cwd[1024];
  getcwd(cwd, sizeof(cwd));
  std::string path = std::string(cwd) + "/libfoo.so";
  void *handle = dlopen(path.c_str(), RTLD_NOW|RTLD_LOCAL);

  std::cout << "handle = " << handle << "\n";
  using FooType = void (*)(const std::type_info &); 
  FooType foo_ptr = reinterpret_cast<FooType>(dlsym(handle, "foo"));

  if(foo_ptr) 
    foo_ptr(typeid(Foo));
  
  if(handle)
    dlclose(handle);
}

void test_variable_template_linking() {
  Bar::v<Foo>.push_back(Foo{});
  // if -rdynamic option is not provided
  // the following assertion will fail.
  assert(Bar::v<Foo>.size() == 2);
  std::cout << "size of v<Foo> = " << Bar::v<Foo>.size() << "\n";
}

int main(void) {
  test_typeinfo();
  test_variable_template_linking();
}
