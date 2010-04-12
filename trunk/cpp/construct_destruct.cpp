#include <iostream>

// construct() constructs a new object in
// a given location using an initial value
//
template <class T1, class T2>
void construct( T1* p, const T2& value ) 
{
  new (p) T1(value);
}

// destroy() destroys an object or a range
// of objects
//
template <class T>
void destroy( T* p ) 
{
  p->~T();
}

template <class FwdIter>
void destroy( FwdIter first, FwdIter last ) 
{
  while( first != last ) 
  {
    destroy( first );
    ++first;
  }
}

// swap() just exchanges two values
//
template <class T>
void swap( T& a, T& b ) 
{
  T temp(a); a = b; b = temp;
}

int main (void)
{
  char *p = static_cast <char *> (::operator new (sizeof (char)));
  construct (p, 'A');
  destroy (p);
  ::operator delete (p);
  return 0;
}


