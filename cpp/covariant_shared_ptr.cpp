#include <iostream>
#include <boost/shared_ptr.hpp>

using namespace boost;

struct null_type {};

class A {
  public:
    typedef null_type super;
};
class B : public A 
{
  public:
    typedef A super;
};

template <class T>
struct wrapper : wrapper<typename T::super>
{
  shared_ptr<T> ptr;
  wrapper(shared_ptr<T> sh) : ptr(sh) {}
  operator shared_ptr<T> () { return ptr; }
};

template <>
struct wrapper<null_type> {};

class X {
    //virtual shared_ptr<A> foo();
    virtual wrapper<A> bar();
    //virtual A zoo();
};

class Y : public X {
    //virtual shared_ptr<B> foo();
    virtual wrapper<B> bar();
    //virtual B zoo();
};

int main(void)
{

}
