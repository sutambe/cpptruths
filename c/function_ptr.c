#include <stdio.h>

typedef void *p;
typedef void *q[1];
typedef int (*w)[1];
typedef void (*r)(void *);
typedef int (*s())[1][1];
typedef void (*t())();
typedef void (*(*u())[1])();
typedef void ( *y (int, void (*)(void *) ) ) ();

int func (int a, int b)
{
   return 0;
}

int main (void)
{
  //typedef int (*FPTR)(float, ...);
  typedef int (*FPTR)();
  FPTR p1 = func;
  p1 (100, 200);
  p1 ("TEST", 200, "TEST");
  printf ("p1 = %x, &func = %x\n", (unsigned) p1, (unsigned) &func);
  
  return 0;
}

