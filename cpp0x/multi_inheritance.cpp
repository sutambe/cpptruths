#include <iostream>


void test1(void) {
class A
{
public:
     virtual ~A() = default;
};
class B : public A
{
public:
     virtual ~B() = default;
};
class C
{
public:
     virtual ~C() = default;
};
class D : public B, public C
{
public:
     virtual ~D() = default;
};
 D *pd = new D();
  std::cout << "pd=" << pd << "\n";

  A *pa = pd;    // No pointer arithmetic pa points to same location as pd.
  std::cout << "pa=" << pa << "\n";

  B *pb = pd;   // No pointer arithmetic pb points to same location as pd.
  std::cout << "pb=" << pb << "\n";

  C *pc = pd;
  std::cout << "pc=" << pc << "\n";

  pd = static_cast<D*>(pa);   // No Pointer Arithmetic performed by compiler, pa and pd points to same location
  std::cout << "pd=" << pd << "\n";

  pd = static_cast<D*>(pb);  // No Pointer Arithmetic performed by compiler, pb and pd points to same location
  std::cout << "pd=" << pd << "\n";

  pd = static_cast<D*>(pc); 
  std::cout << "pd=" << pd << "\n";
}

void test2(void) {
class A
{
    int a;
public:
     virtual ~A() = default;
};
class B : public virtual A
{
    int b;
public:
     virtual ~B() = default;
};
class C : public virtual A
{
    int c;
public:
     virtual ~C() = default;
};
class D : public B, public C
{
    int d;
public:
     virtual ~D() = default;
};
  D *pd = new D();
  std::cout << "pd=" << pd << "\n";

  A *pa = pd;    // No pointer arithmetic pa points to same location as pd.
  std::cout << "pa=" << pa << "\n";

  B *pb = pd;   // No pointer arithmetic pb points to same location as pd.
  std::cout << "pb=" << pb << "\n";

  C *pc = pd;
  std::cout << "pc=" << pc << "\n";

  pd = static_cast<D*>(pb);  // No Pointer Arithmetic performed by compiler, pb and pd points to same location
  std::cout << "b to d pd=" << pd << "\n";

  pd = static_cast<D*>(pc); 
  std::cout << "c to d pd=" << pd << "\n";

  //pd = static_cast<D*>(pa);   // No Pointer Arithmetic performed by compiler, pa and pd points to same location
  //pb = static_cast<B*>(pa);   // No Pointer Arithmetic performed by compiler, pa and pd points to same location
  //pc = static_cast<C*>(pa);   // No Pointer Arithmetic performed by compiler, pa and pd points to same location
  //std::cout << "pd=" << pd << "\n";

  pd = dynamic_cast<D*>(pa);   // No Pointer Arithmetic performed by compiler, pa and pd points to same location
  //pb = static_cast<B*>(pa);   // No Pointer Arithmetic performed by compiler, pa and pd points to same location
  //pc = static_cast<C*>(pa);   // No Pointer Arithmetic performed by compiler, pa and pd points to same location
  std::cout << "a to d pd=" << pd << "\n";

  void *v = pa;
  pd = dynamic_cast<D*>(static_cast<A *>(v));
  std::cout << "v to d pd=" << pd << "\n";
}

void test3(void) {
class Object { 
  public:
    virtual ~Object() = default;
};
class A : public Object
{
public:
     virtual ~A() = default;
};
class B : public A
{
public:
     virtual ~B() = default;
};
class D : public B
{
public:
     virtual ~D() = default;
};
 D *pd = new D();
  std::cout << "pd=" << pd << "\n";

  A *pa = pd;    // No pointer arithmetic pa points to same location as pd.
  std::cout << "pa=" << pa << "\n";

  B *pb = pd;   // No pointer arithmetic pb points to same location as pd.
  std::cout << "pb=" << pb << "\n";

  //C *pc = pd;
  //std::cout << "pc=" << pc << "\n";

  pd = static_cast<D*>(pa);   // No Pointer Arithmetic performed by compiler, pa and pd points to same location
  std::cout << "pd=" << pd << "\n";

  pd = static_cast<D*>(pb);  // No Pointer Arithmetic performed by compiler, pb and pd points to same location
  std::cout << "pd=" << pd << "\n";

  //pd = static_cast<D*>(pc); 
  //std::cout << "pd=" << pd << "\n";
    
  void *v = pd;
  //Object *o = static_cast<Object *>(v);
  pb = static_cast<B *>(v);
  std::cout << "pb=" << pb << "\n";
}

void test4(void) {
class Object { 
  public:
    virtual ~Object() = default;
};
class A : public virtual Object
{
public:
     virtual ~A() = default;
};
class B : public A
{
public:
     virtual ~B() = default;
};
class C : public virtual Object
{
public:
     virtual ~C() = default;
};
class D : public B, public C {
public:
     virtual ~D() = default;
};
  D *pd = new D();
  std::cout << "pd=" << pd << "\n";

  A *pa = pd;    // No pointer arithmetic pa points to same location as pd.
  std::cout << "pa=" << pa << "\n";

  B *pb = pd;   // No pointer arithmetic pb points to same location as pd.
  std::cout << "pb=" << pb << "\n";

  C *pc = pd;
  std::cout << "pc=" << pc << "\n";

  //pd = static_cast<D*>(pa);   // No Pointer Arithmetic performed by compiler, pa and pd points to same location
  //std::cout << "pd=" << pd << "\n";

  pd = static_cast<D*>(pb);  // No Pointer Arithmetic performed by compiler, pb and pd points to same location
  std::cout << "pd=" << pd << "\n";

  pd = static_cast<D*>(pc); 
  std::cout << "pd=" << pd << "\n";
    
  void *v = pc;
  Object *o = static_cast<Object *>(v);
  pd = dynamic_cast<D *>(o);
  std::cout << "o=" << o << ", from void* pd=" << pd << "\n";
}

int main(void) {
  //test1();   
  test2();
  //test3();
  //test4();
}
