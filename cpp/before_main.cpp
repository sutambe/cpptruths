#include <stdio.h>
//#pragma startup fun

int fun()
{
  printf("Before main()\n");
  return 10;
}

/* Not allowed in C. Constant initializer is expected.*/
int result = fun ();

int main()
{
  // There is no statment.
  printf ("%d\n",result);
  return 0;
}

