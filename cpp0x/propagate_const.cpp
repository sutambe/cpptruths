#include <iostream>
#include <memory>
#include <experimental/propagate_const>
 
struct X
{
    void g() const { std::cout << "g (const)\n"; }
    void g() { std::cout << "g (non-const)\n"; }
};

template <class Ptr>
struct Y
{
    Y(Ptr x, Ptr y) : xptr(std::move(x)), yptr(std::move(y)) { }
 
    void f() const
    {
        std::cout << "f (const)\n";
        xptr->g();
        yptr->g();
        element_type(xptr.get());
        element_type(yptr.get());
        std::experimental::propagate_const<Ptr> another(new typename Ptr::element_type(*xptr));
        //xptr = std::move(another);
        //get_underlying(xptr) = Ptr(new typename Ptr::element_type(*xptr));
        //get_underlying(xptr).reset();
        //typename Ptr::element_type const * e = xptr;
    }
 
    void f()
    {
        std::cout << "f (non-const)\n";
        xptr->g();
        yptr->g();
        element_type(xptr.get());
        element_type(yptr.get());
        std::experimental::propagate_const<Ptr> another(new typename Ptr::element_type(*xptr));
        xptr = std::move(another);
    }
    
    void element_type(typename Ptr::element_type *)  const {
      std::cout << "non-const element_type ptr\n";  
    }
 
    void element_type(typename Ptr::element_type const *) const {
      std::cout << "const element_type ptr\n";  
    }
 
    std::experimental::propagate_const<Ptr> xptr;
    Ptr yptr;
};

struct P { 
  void modify() {
    std::cout << "modify\n";
  }
  void modify() const {
    std::cout << "no-modify\n";
  }
};

struct Q {  
  std::experimental::propagate_const<const P*> xptr;
  explicit Q(P* p) : xptr(p) {}
  void modify() const {
    xptr->modify();
  }
};


int main()
{
    const Q q(new P);
    q.modify(); 

    Y<std::unique_ptr<X>> y1(std::make_unique<X>(), std::make_unique<X>());
    y1.f();
    const Y<std::unique_ptr<X>> cy1(std::make_unique<X>(), std::make_unique<X>());
    cy1.f();
    std::cout << "\n";
    //y1 = cy1;
    
    //Y<std::shared_ptr<X>> y2(std::make_shared<X>(), std::make_shared<X>());
    //y2.f();
    //const Y<std::shared_ptr<X>> cy2(std::make_shared<X>(), std::make_shared<X>());
    //cy2.f();
    
    
    
    //Y<nullptr_t> y3(nullptr, nullptr);
    //nullptr_t n; *n;
    
    
}
