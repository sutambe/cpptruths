#include <stdio.h>

int main(void)
{
  int a,b;
  a=5,b=6;
  printf("%d\n",a+++b);
  // Maximal munch???
  a=5,b=6;
  printf("%d\n",a+ ++b);

  return 0;
}

