#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <new>

struct X {};

class C
{
  public:
    C ()     { 
      std::cout << "C::C()\n";
      throw X(); 
    }
    ~C () {
      std::cout << "C::~C()\n";
    }
   
    // If you declare one operator new, it supresses automatic generation of 
    // other operator new. So to prevent dynamic memory allocation just make
    // this one private.
    static void * operator new (size_t size)
    {
      std::cout << "C::operator new (size_t size) " << size << "\n";
      return malloc (size);
    }
    
    // A matter of style: size can be removed from the destructor below.
    static void operator delete (void *p, size_t size)
    {
      std::cout << "C::operator delete (void *p, size_t size)\n";
      free (p);
    }
    
    static void * operator new (size_t size, const X &x)
    {
      std::cout << "C::operator new (size_t size, X x) " << size << " " << std::hex << &x << "\n";
      return malloc (size);
    }
    
    static void operator delete (void *p, const X &x)
    {
      std::cout << "C::operator delete (void *p, X x) " << std::hex << &x << "\n";
      free (p);
    }
    
    static void * operator new (size_t size, std::nothrow_t)
    {
      std::cout << "C::operator new (size_t size, std::nothrow_t) " << size << "\n";
      return malloc (size);
    }
    
    static void operator delete (void *p, std::nothrow_t)
    {
      std::cout << "C::operator delete (void *p, std::nothrow_t)\n";
      free (p);
    }
    
    static void * operator new (size_t size, void *p)
    {
      std::cout << "C::operator new (size_t size, void *p) " << size << "\n";
      return p;
    }
    
    static void operator delete (void *p, void *)
    {
      std::cout << "C::operator delete (void *p, void *p)\n";
      free (p);
    }
    
    static void * operator new [] (size_t size)
    {
      std::cout << "C::operator new [] (size_t size) " << size << "\n";
      return malloc (size);
    }
    
    static void operator delete [] (void *p)
    {
      std::cout << "C::operator delete [] (void *p)\n";
      free (p);
    }
    
    static void * operator new [] (size_t size, const X &x)
    {
      std::cout << "C::operator new [] (size_t size, const X &x) " << size << "\n";
      return malloc (size);
    }
    
    static void operator delete [] (void *p, const X &x)
    {
      std::cout << "C::operator delete [] (void *p, const X &x)\n";
      free (p);
    }
     
};

int main (void)
  try
{

  {
    C *c1 = new C;
    std::cout << "Deleting c1\n";
    delete c1;
  }

  {
    void *mem = malloc (sizeof(C));
    C *c4 = new (mem) C;
    std::cout << "Deleting c4\n";
    c4->~C();
    //If C:: is not given below then global placement delete is invoked.
    //C::operator delete (c4, mem);
  }
  

  {
    X x;
    C *c2 = new (x) C;
    std::cout << "Deleting c2\n";
    c2->~C();
    // If C:: is not given below then there is no standard global delete
    // operator for X &x so the compiler throws an error at us.
    C::operator delete (c2, x);
  }
  
  {
    C *c3 = new (std::nothrow) C;
    std::cout << "Deleting c3\n";
    c3->~C();
    // If C:: is not given below then global nothrow delete is invoked.
    C::operator delete (c3, std::nothrow);
  }

  {
    C *c5 = new C[3];
    std::cout << "Deleting c5\n";
    delete [] c5;
  }
  
  {
    X x;
    C *c6 = new (x) C[3];
    std::cout << "Deleting c6\n";
    C::operator delete [] (c6, x);
  }
  
  return 0;
}
catch (...)
{
  std::cout << "Exception caught.\n";
  return 1;
}

