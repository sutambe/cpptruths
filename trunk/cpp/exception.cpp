#include <iostream>

int t(int i) throw (std::bad_alloc, int)
{
  try
    {
      throw std::bad_alloc();
    }
  catch (std::bad_alloc)
    {
      //throw;
    }
  return i;
}

class mystring : public std::string
{
public:
  mystring (const char *str) : std::string (str) {}
  virtual ~mystring ()
  {
    std::cout << "mystring destructor : " << this->c_str() << std::endl;
  }
};

class myclass
{
public:

  myclass (int i) try : str ("ASDF"), value (t(i)), str2(new mystring ("PQRS"))
  {
    int local;
    local = 2*i;
    //throw local=2*i;
    std::cout << "before calling func" << std::endl;
    func();
    std::cout << "After calling func" << std::endl;
  }
  catch (int e)
  {
    std::cout << "Constructor Exception: " << e << std::endl;
    this->value = 10;
  }

  void func ()
    {
      try {
        throw 10;
        }
      catch (int)
        {
          std::cout << "cought exception in func" << std::endl;
        }
    }

  void * operator new (size_t nbytes)
  {
    std::cout << "Invoking new" << std::endl;
    return (myclass *) ::operator new (nbytes);
  }

  void operator delete (void *ptr)
  {
    std::cout << "Invoking delete" << std::endl;
    :: operator delete (ptr);
  }

  ~myclass (void)
  {
    delete str2;
    std::cout << "Destructor" << std::endl;
  }

private:
  mystring str;
  int value;
  mystring *str2;

};

int main (void)
{
  try {
    myclass *m =  new myclass (100);
    delete m;
  }
  catch (int e)
  {
    std::cout << "Main Exception: " << e << std::endl;
  }
  catch (std::exception &e)
  {

  }
  
  return 0;
}

