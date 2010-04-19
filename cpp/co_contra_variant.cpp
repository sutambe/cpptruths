#include <iostream>

struct A {
  virtual ~A() {}
};

struct B : A {

};

struct Shape {
  virtual A * do_something (B* b)  { 
    std::cout << "Shape::do_something()\n";
    return b; 
  }
  virtual ~Shape() {}
};

struct Circle : Shape {
  virtual B * do_something (A* x)  
  { 
    // This function has covariant return type and 
    // contravarint parameter type. Some languages support both.
    // C++ supports only covarint return types. 
    // In C++ these are two different overloads.
    std::cout << "Circle::do_something()\n";
    return dynamic_cast<B *>(x); 
  }
};

A * f(B *);
B * g(A *);

int main(void)
{
  B b;
  Shape * shape = new Circle();
  shape->do_something(&b);
  delete shape;

  typedef A * F(B *);
  typedef B * G(A *);
  F *pf = f;
  G *pg = g;
  pf = pg; // This conversion is invalid in C++ although subtype relationship supports it.
}
