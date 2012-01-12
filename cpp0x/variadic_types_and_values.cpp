#include <cstdio>

//template <class T, class U, T t, U u>
template <class... T, T... t>
void func()
{
  //printf("%d\n", sizeof...(U));
}

int main(void)
{
  //func<int, double, 10, 10.10>();
  func<char, int, 'A', 10>();
}

