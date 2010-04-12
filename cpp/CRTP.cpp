#include <iostream>

template <class Derived>
struct base {
  int i;
  void foo() {
    static_cast<Derived *>(this)->foo();
    std::cout << this << std::endl; // Prints the same as below.
    std::cout << static_cast<Derived *>(this) << std::endl;
  };
};

struct my_type : base<my_type> {
  float j;
  void foo() // required to compile.
  {
    std::cout << "my_type::foo()" << std::endl;
  }
};

struct your_type : base<your_type> {
  double d;
  void foo() // required to compile.
  {
    std::cout << "your_type::foo()" << std::endl;
  }
};

template <class T> // T is deduced at compile-time
void bar(base<T> & obj) {
    obj.foo(); // will do static dispatch
}

int main(void)
{
  my_type m;
  your_type y;
  bar(m);
  bar(y);
}

