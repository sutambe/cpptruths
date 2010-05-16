#include <iostream>
#include <string>
#include <vector>
#include <cstring>

class X 
{
    char * str;
  public:
    X() : str(0)
    {
      std::cout << "X::X()\n";
    }
    X(const char * p) 
      : str(p ? new char [strlen(p) + 1] : 0)
    {
      if(str)
        strcpy(str, p);
      std::cout << "X::X(const char *)\n";
    }
    X(const X & x) 
      : str(x.str ? new char [strlen(x.str) + 1] : 0)
    {
      if(str)
        strcpy(str, x.str);
      std::cout << "X::X(const X &)\n";
    }
    X & operator = (const X & x) 
    {
      std::cout << "X::operator = (const X &)\n";
      X(x).swap(*this);
      return *this;
    }
    void swap (X & x)
    {
      std::swap(this->str, x.str);
    }
#ifdef __GXX_EXPERIMENTAL_CXX0X__     
    X(X && x) 
      : str(x.str) 
    {
      x.str = 0;
      std::cout << "X::X(X &&) move constructor\n";
    }
    X & operator = (X && x) 
    {
      std::cout << "X::operator = (X &&) move assignment\n";
      std::swap(this->str, x.str);
      return *this;
    }
#endif // __GXX_EXPERIMENTAL_CXX0X__    
    ~X () 
    { 
      std::cout << "X::~X (), str = " << (str ?: "0") << std::endl;
      delete [] str;
    }
    void print(std::ostream & o) const
    {
      o << str;
    }
    friend std::ostream & operator << (std::ostream &, X const & x);
};

std::ostream & operator << (std::ostream & o, X const & x)
{
  x.print(o);
  return o;
}

/*
namespace std {
void swap (::X & x, ::X & y) {
  x.swap(y);
}
}
*/

X xmaker()
{
  X x("Nashville");
  return x;
}

void f(const std::string &)
{
  std::cout << "f(const &)\n"; 
}

void f(std::string &&)
{
  std::cout << "f(&&)\n"; 
}

int main (void)
{
  const char * str = "Test";
  f(str);
/* 
  X x("Sumant");
  X y("Tambe");
  std::cout << "Calling swap\n";
  std::swap(x,y);
  std::cout << "x = " << x << std::endl;
  std::cout << "y = " << y << std::endl;
*/
 /* 
  //X v("Vanderbilt");
  std::vector<X> v1;
  //v1.push_back(v);

  //X n("Nashville");
  std::vector<X> v2;

  std::cout << "****************** Construction of test objects complete ***************\n";
  
  //v2.push_back(std::move(n));
  v2.push_back(xmaker());

  //std::swap(v1,v2);
  v1 = std::move(v2);

  std::cout << "********************** Call stack unwinds below ************************\n";
*/
}

