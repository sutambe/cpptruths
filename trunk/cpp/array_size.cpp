#include <cstdio>

size_t func(float src[3][3])
{
  return sizeof(src);
}

int main(void)
{
  float src[3][3] = {{0}};
  printf("%d and %d\n", func(src), sizeof(src)); // prints 4 and 36

  return 0;
}
