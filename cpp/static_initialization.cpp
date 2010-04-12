#include <iostream>
using namespace std;
struct Bar {
    Bar () {
          cout << "Bar::Bar()\n";
            }
      void f () {
            cout << "Bar::f()\n";
              }
};
struct Foo {
    Foo () {
          bar_.f ();
            }
      static Bar bar_;
};

Foo f;
Bar Foo::bar_;

int main () {}

