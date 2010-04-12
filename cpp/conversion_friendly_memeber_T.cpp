#include <algorithm>
#include <iostream>

template <class T>
class Array
{
  public:
    Array () {}
    Array (Array const & a)
    {
      std::copy (a.array_, a.array_ + SIZE, array_);
    }

    template <class U>
    Array (Array <U *> const & a)
    {
      std::copy (a.array_, a.array_ + SIZE, array_);
    }

    enum {  SIZE = 10 };
    T array_[SIZE];
};

template <class T>
class Ptr
{
  public:
    Ptr () {}

    Ptr (Ptr const & p)
      : ptr (p.ptr) 
    {
      std::cout << "This is copy-constructor\n";
    }
    
    // A conversion friendly member template constructor.
    template <class U>
    Ptr (Ptr <U> const & p)
      : ptr (p.ptr) // Implicit conversion from U to T must be allowed.
    {
      std::cout << "conversion friendly member template "
                   "constructor is not a copy-constructor\n";
    }

    // A conversion friendly member template assignment operator.
    template <class U>
    Ptr & operator = (Ptr <U> const & p)
    {
      ptr = p.ptr; // Implicit conversion from U to T must be allowed.
      std::cout << "conversion friendly member template assignment "
                   "operator is a not copy-assignment operator\n";
      return *this;
    } 

    T *ptr; 
};

class B
{
  public:
    void operator = (B *) {}
};

class D : public B
{};

int main (void)
{
  Array <D *> d;
  Array <B > b (d);
  
  Ptr <D> d_ptr;
  Ptr <B> b_ptr (d_ptr);
  b_ptr = d_ptr;

  return 0;
}
