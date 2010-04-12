#include <iostream>
using std::cout;

struct A {
    A (int x) { cout << x << std::endl; }
};

struct B : virtual A {
    B () :A(1) {}
};

struct C : virtual A {
    C () :A(2) {}
};

struct D : B, C {
  D() : A(10) {}
};

int main () {
    D d;
    B b;
    C c;
}
