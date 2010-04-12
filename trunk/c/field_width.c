#include <stdio.h>
int main(void)
{
  char *p="wipro";
  int n = 2;
  printf("%.*s %3.*d\n",n,p,9,12345);
  return 0;
}
