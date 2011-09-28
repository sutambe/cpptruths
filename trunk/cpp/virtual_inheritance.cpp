#include <iostream>
using std::cout;

struct A {
    A (int x) { cout << x << std::endl; }
    virtual ~A() {}
};

struct B : virtual A {
    B () :A(1) {}
};

struct C : virtual A {
    C () :A(2) {}
};

struct D : B, C {
  D() : A(10) {}
  void bar() { std::cout << "999\n"; }
};

int main () {
    B b;
    C c;
    A *a = new D();
    D *d = dynamic_cast<D *>(a);
    d->bar();
}
