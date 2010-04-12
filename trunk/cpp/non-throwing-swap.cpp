#include <iostream>

namespace zoo {

template <class T>
class X
{
    T t;
  public:
    void swap (X & x) { using std::swap; swap (t, x.t); }
    // Above funtion oculd be slower if std::swap is not optimal for certain
    // large types. 
};
/*
template <class T>
void swap (X<T> & x1, X<T> & x2)
{
  x1.swap (x2);
  std::cout << "namespace swap\n";
}
*/
}

int main (void)
{
  zoo::X<int> x1, x2;
  using std::swap;
  swap (x1,x2);
  // Using unqualified swap works with koenig lookup but only if the author of
  // the template has defined a swap in its own namespace. Otherwise a
  // compilation error is flagged.

  return 0;
}
