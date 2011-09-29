#include <cstdio>
#include <typeinfo>

struct base { virtual ~base() {} };
struct derived : public base {};

template <class T>
T & nonnull(T& t)
{
  return t;
}

int main(void)
{
  try {
    base *b = new derived();
    b = 0;
    printf("%s\n", typeid(1 ? *b : *b).name());  
    printf("%s\n", typeid(nonnull(*b)).name());  
  }
  catch(const std::exception & e)
  {
    printf("Err: %s\n", e.what());
  }
}
