#include <iostream>
#include <string>

class X {
  public:
    X(bool b) { if (b) throw 10; };
    static void * operator new (size_t size)
    {
      return malloc (size);
    }
/*    static void operator delete (void *p)
    {
      ::operator delete (p);
    }
    static void * operator new (size_t size, std::nothrow_t) throw ()
    {
      try {
          return operator new(size); // forward to pair 1
      }
      catch (...) {}
      return 0;
    } 
    static void operator delete (void *p, std::nothrow_t) throw ()
    {
      try { X::operator delete (p); }
      catch(...) {}
    }*/
  private:
    int c[100];
};

int main (void)
{
  try 
  {
    //void * ptr = ::operator new (sizeof (X));
    //X *x = new (ptr) X(true);
    //X * x2 = new (std::nothrow) X (true);
    X * x3 = new X (true);
  }
  catch (...)
  {
    std::cout << "Exception\n";
  }
  return 0;
}
