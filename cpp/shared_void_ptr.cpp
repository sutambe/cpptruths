//#include <tr1/memory>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <exception>
#include <cstdlib>

struct A 
{
  ~A() {  // Note the destructor is not virtual.
    std::cout << "A::~A()\n";
  }
};

struct B : A
{
  ~B() {
    std::cout << "B::~B()\n";
  }
};

void *operator new (size_t size)
{
  static int i = 0;
  std::cout << "size = " << size << std::endl;
  if(++i == 4)
    throw std::bad_alloc();
  void *ptr = malloc(size);
  return ptr;
}

void operator delete (void *p)
{
  free(p);
}

template <class T>
struct MySmartPtr
{
  int *count;
  MySmartPtr(T *t)
    try
    : count(new int(1))
  {
  }
  catch(std::bad_alloc) {
    delete t;
  }
  ~MySmartPtr()
  {
    delete count;
  }
};

int main(void)
{
  try {
    //std::tr1::shared_ptr<void> vp (new double);
    boost::shared_ptr<void> vp (new double);
    //std::tr1::shared_ptr<A> ap (new B); // Destructor of B is called.
    MySmartPtr<B> msptr (new B); // Destructor of B is called.
  }
  catch(std::bad_alloc) {
    std::cout << "Caught exception = bad_alloc\n";
  }
}

