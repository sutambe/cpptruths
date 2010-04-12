#include <list>
#include <vector>

struct Foo 
{ // does not have default constructor.
  Foo (int i) {} 
};
int main(void) 
{
  std::list<Foo> l(10, Foo(5));
  std::vector<Foo> v(10, Foo(6));
  v.reserve(100);
  std::list<Foo> l2;
}
