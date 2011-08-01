#include <memory>
#include <stdexcept>
#include <cstdio>

struct X
{
  ~X() { 
    printf("Destroying X\n");
  }
};


struct TestException : std::runtime_error
{
  TestException(const char *str, 
                std::auto_ptr<X> xptr = std::auto_ptr<X>()) 
    : std::runtime_error(str),
      ptr(xptr)
  {}
  
  TestException(const TestException &t) 
    : std::runtime_error(t)
  {
    ptr = const_cast<TestException &>(t).ptr;
  }
  virtual ~TestException() throw() {}
  std::auto_ptr<X> get_ptr() { return ptr; }
  
  std::auto_ptr<X> ptr;
};

void func(void)
try 
{
    std::auto_ptr<X> xptr (new X);
    throw TestException("Test Exception", xptr);
}  
catch(TestException & te)
{
  printf("Caught TestException\n");
  throw;
}

int main(void)
try
{
  func();
}
catch(std::exception & e)
{
  printf("%s\n", e.what());
  TestException & te = dynamic_cast<TestException &> (e);
  std::auto_ptr<X> xptr = te.get_ptr();
  printf("Obtained X\n");
}
