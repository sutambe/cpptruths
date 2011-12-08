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

int main(void)
{
  Bar b;
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
