#include <iostream>
#include <iomanip>
using namespace std;
class B {};
class D1 :virtual public B{};
class D2 :virtual public B{};
class D : public D1, public D2 {} ;
template <class T> void func (T a ) {}
//template <class T> void func <char *> (T *a) {}
class o
{
  public:
  int p;
  class i
  {
    //i () { o obj; obj.p = 10;}
  };
  //o () { i obj;}
};
int f () {}
char f () {}
int main(void)
{
  int z = f ();
  int o::*iptr = &o::p;
  int *(*p)[5];
//  p = new int [3]*[5];
  p = new int *[3][5];
//  p = new int (*)[3][5];
//  p = new int [3][5]*;
//  p = new int (*[3])[5];
  double pi = 3.1415;
  cout.precision (3);
  cout << pi << endl;
  std::type_info const &t = (typeid (std::string));
}

