#include <boost/utility/enable_if.hpp> 
#include <iostream>

template <class T, class U, class Actual>
struct BothSame
{
  enum { value = std::is_same<typename std::remove_reference<T>::type, Actual>::value && 
                 std::is_same<typename std::remove_reference<U>::type, Actual>::value }; 

  typedef BothSame type;
};

struct matrix 
{
  matrix & operator += (const matrix & m) {
    return *this;
  }
};

#define IS_RVALUE(TYPE, VAR) (std::is_rvalue_reference<decltype(std::forward<TYPE>((VAR)))>::value)

template <class T, class U>
typename boost::enable_if<BothSame<T, U, matrix>, 
                          typename std::remove_reference<T>::type>::type 
operator + (T &&a, U && b)
{
  if(IS_RVALUE(T, a))
  {
    std::cout << "lhs is rvalue\n";
    matrix r (std::move(a));
    r += b;
    return r;
  }
  else if(IS_RVALUE(U, b))
  {
    std::cout << "rhs is rvalue\n";
    matrix r (std::move(b));
    r += a;
    return r;
  }
  else
  {
    std::cout << "both lvalue\n";
    matrix r(a);
    r += b;
    return r;
  }
}

int main(void)
{
  matrix a, b;
  a + b;
  matrix() + b;
  a + matrix();
  matrix() + matrix();

  return 0;
}
