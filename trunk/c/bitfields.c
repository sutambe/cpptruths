#include <stdio.h>

#define TRUE  1
#define FALSE 0

struct A
{
  int p:1;
  unsigned int q:1;
};

int main (void)
{
  struct A a;
  a.p = 1;
  a.q = 1;
  unsigned int i = a.p;
  printf("Values = %d, %d, %u\n", a.p, a.q, i);
  
  if (a.p == TRUE)
    printf("They are the same.\n");
  else
    printf("They are not the same.\n");

  return 0;
}
