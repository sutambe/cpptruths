#include <stdio.h>

#define SIZEOF(var) ((char *)(&var + 1) - (char *)&var)

void myfunc(void)
{}

int main (void)
{
   int i;
   void (*p)(void) = myfunc;
   printf("original = %d\n", sizeof(myfunc));
   printf("simulated = %d\n", SIZEOF(myfunc));
   return 0;
}
/*
gcc sizeof-macro.c -o sizeof-macro -Wall -ansi -pedantic
*/

