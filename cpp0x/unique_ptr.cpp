#include <memory>
#include <iostream>

class Foo;

class Bar {
  std::unique_ptr<Foo> foo_ptr;
public:
  Bar();
  ~Bar() {
  }  
};

void func(std::unique_ptr<int> ptr)
{
  ptr.reset();
}
namespace CORBA {
template<class T>
std::unique_ptr<T> transfer(T *t)
{
  return std::unique_ptr<T>(t);
}
}

int main(void)
{
  Bar b;
  int i = 0;
  func(CORBA::transfer(&i));
}

class Foo
{
  int z;
public:
  virtual ~Foo(){}
};

class FooDerived : public Foo {
  ~FooDerived() {
    std::cout << "~FooDerived\n";
  }
};

Bar::Bar () 
 : foo_ptr(new FooDerived)
{}
