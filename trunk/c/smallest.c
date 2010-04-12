#include <stdio.h>

int small ();
int main (int argc, char *argv[])
{
  int a = atoi(argv[1]);
  int b = atoi(argv[2]);
  int c = atoi(argv[3]);
  printf ("Smallest among  %d, %d, %d = %d\n",a,b,c, small(a,b,c));
  return 0;
}

int small(int a, int b, int c)
{
  if(a && b && c)
    return (small(--a,--b,--c))+1;
  return 0;
}

