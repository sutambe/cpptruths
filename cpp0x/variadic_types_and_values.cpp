#include <cstdio>
#include <functional>



//template <class T, class U, T t, U u>
//template <class... T, T... t>
template <class T, T* t>
void func()
{
  //printf("%d\n", sizeof...(U));
  typename std::reference_wrapper<T>::result_type ret;
  typename std::reference_wrapper<T>::argument_type arg;
}

int main(int argc, char * argv[])
{
  //func<int, double, 10, 10.10>();
  func<decltype(main), main>();
  //func<char, int, 'A', 10>();
}

