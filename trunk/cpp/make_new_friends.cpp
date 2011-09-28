#include <iostream>

using namespace std;

// Forward declarations
template<class T> class Box;
template<class T>
  ostream& operator<<(ostream&, const Box<T>&);

template <typename T>
class Person
{
  T t;
};

template<class T> 
class Box 
{
  T value;
public:
  Box(const T& t) : value(t) { } 
    
  friend ostream& operator<< <>(ostream&, const Box<T>&);
  friend class Person<T>;
  
  struct wrapper {
    typedef T Type;
  };

  friend class wrapper::Type;  // Supported by g++ but not by VC2010, Comeau
  //friend class T; // not supported on C++03 compilers 
};

template<class T>
ostream& operator<<(ostream& os, const Box<T>& b) 
{
  return os << b.value;
}

struct Foo 
{
  void func() {
    Box<Foo> f(*this);
    f.value = *this;
  }
};

int main(void)
{
  /*
  Box <int> b (10);
  std::cout << b << std::endl;
  Box <double> c (10.345);
  std::cout << c << std::endl;
  */
  Box<Foo> bfoo(Foo());
  return 0;
}

