#include <functional>
#include <boost/function.hpp>

class A {
  A();
  public:
    A toCPD() const { return A(); }
};

template<typename T>
class Ev 
{
  public:
    //typedef typename std::result_of<T::toCPD()>::type D;
    //typedef decltype(std::declval<T>().toCPD()) D;
    //typedef typename boost::function<decltype(T::toCPD)>::result_type D;
};

int main(void)
{
  Ev<A> ev;
  boost::function<A (A *)> f(&A::toCPD);
}
