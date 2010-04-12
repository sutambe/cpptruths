#include <iostream>

template <class T> class Box;

template <class T> void foo(Box<T> const & b);
template <class T>
std::ostream & operator << (std::ostream & o, Box<T> const & b);

template <class T>
class Test
{
  T a;
};

template<class T> 
class Box 
{
//public:
  T t;
public:
  Box(const T& theT) : t(theT) {}
  friend std::ostream & operator << <> (std::ostream & o, Box<T> const & b);
  //{		return o << b.t;   }
  friend void foo <T> (Box<T> const & b);
  friend void bar (Box<T> const & b);
  template <class U> friend void zoo (Box<U> const & b);
  friend class Test<T>;
  friend class Z;
  class Z { T b; };
  //{ std::cout << "T " << b.t << std::endl; }
  //friend void bar (Box<T *> const & b)
  //{ std::cout << "ptr " << b.t << std::endl; }
  //friend void bar (Box<double> const & b)
  //{ std::cout << "double " << b.t << std::endl; }
  //{ std::cout << b.t; << std::endl; }

};

template <class T>
std::ostream & operator << (std::ostream & o, Box<T> const & b)
{ 	return o << b.t << ":"; }

template <>
std::ostream & operator << (std::ostream & o, Box<double> const & b)
{ 	return o << "DOUBLE " << b.t << ":"; }

template <class T>
void foo(Box<T> const & b)
{
     Box <float> fb (40.40f);
     std::cout << b.t << std::endl; 
     //std::cout << fb.t << std::endl; 
}

template <>
void foo(Box<char> const & b) { std::cout << b.t << std::endl; }

void bar(Box<double> const & b)
{ std::cout << "double " << b.t << std::endl; }
void bar(Box<int> const & b)
{ std::cout << "int " << b.t << std::endl; }
void bar(Box<int *> const & b)
{ std::cout << "int * " << b.t << std::endl; }

template <class U> 
void zoo (Box<U> const & b)
{
  Box<float> ib(50.50);
  std::cout << "zoo " << b.t << ib.t << std::endl;
}

int main(void)
{
    Box<int> b(10);
	std::cout << b << std::endl;
    Box<double> d(10.1);
	::operator << (std::cout, d) << std::endl;
	//std::cout << d << std::endl;
	::foo(d);
	::bar(d);
	Box<int *> p((int *)0);
	bar(p);
	::zoo(b);
	return 0;
}
