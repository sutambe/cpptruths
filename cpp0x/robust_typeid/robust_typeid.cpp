#include <typeinfo>
#include <iostream>
#include <string>
#include <unistd.h>
#include <dlfcn.h>

#include "foo.h"

int main(void) {
  char cwd[1024];
  getcwd(cwd, sizeof(cwd));
  std::string path = std::string(cwd) + "/libfoo.so";
  void *handle = dlopen(path.c_str(), RTLD_LAZY |RTLD_LOCAL);
  std::cout << "handle = " << handle << "\n";
  using FooType = void (*)(const std::type_info &); 
  FooType foo_ptr = reinterpret_cast<FooType>(dlsym(handle, "foo"));
  //FooType foo_ptr = reinterpret_cast<FooType>(dlsym(handle, "_Z3fooRKSt9type_info"));

  if(foo_ptr) 
    foo_ptr(typeid(Foo));
  
  if(handle)
    dlclose(handle);
}
