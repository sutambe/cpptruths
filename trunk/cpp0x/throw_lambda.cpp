#include <iostream>
#include <typeinfo>

template <class T>
int foo(T t)
{
  try {
    throw t;
  }
  catch (T) {
    std::cout << typeid(T).name() << std::endl;
  }
  catch(...) {
    std::cout << "caught ...\n";
  }
  return 0;
}


int main(void)
{
  try {
    foo([]{});
  }
  catch(void *v) {
    std::cout << "caught lambda\n";
  }
  catch(...) {
    std::cout << "caught ...\n";
  }

}
