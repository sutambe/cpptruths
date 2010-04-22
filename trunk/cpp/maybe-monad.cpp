#include<iostream>
#include<boost/variant.hpp>

using namespace std;

class Nothing {}; 
template <class T> class Just 
{
  public:
    T val;
    Just(T x) : val(x){}; 
};

template<class T, class F> 
boost::variant<Nothing, Just<T> > 
operator >>= (boost::variant<Nothing, Just<T> > x, F f)
{
    return (boost::get<Nothing>(&x)==NULL) 
           ? f(boost::get<Just<T> >(x).val) 
           : Nothing();
}

template <class T> 
boost::variant<Nothing, Just<T> >
Return(T x) 
{ 
  return Just<T>(x); 
}

boost::variant<Nothing, Just<int> > 
inc(int x) 
{
  return Return<int>(x+1);
}

int main(int argc, char* argv[])
{
    boost::variant<Nothing, Just<int> > foo,bar;
    
    foo = Just<int>(9);
    (foo >>= inc) >>= inc; 

    bar = Nothing();
    (bar >>= inc) >>= inc;
    
    return 0;
}
