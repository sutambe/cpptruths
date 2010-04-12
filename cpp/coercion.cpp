#include <vector>
#include <list>
#include <memory>
using namespace std;

template <class T>
class Vector
{
  public:
    T t;
    template <class U>
    Vector<T> & operator = (const Vector<U> & v)
    {
      t = v.t;
      return *this;
    }
};

int main (void)
{
  Vector<int *> v1;
  Vector<const int *> v2;
  //std::vector<int> sv1;
  //std::vector<const int> sv2(10);
  std::list<int *> l1;
  std::list<const int *> l2;
  
  //sv2 = sv1;  // Error!
  l2 = l1;
  v2 = v1;  // Error!
  auto_ptr<int> a;
  auto_ptr<const int> b;
  b = a;

  return 0;
}
