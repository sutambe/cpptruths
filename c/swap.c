#include <stdio.h>
#include <stdlib.h>
int main (int argc, char *argv[])
{
  int a = atoi(argv[1]);
  int b = atoi(argv[2]);
  printf ("Original a = %d, b = %d\n", a, b);
  
  // Following statemenst are compiler dependent because
  // the order of expression evaluation is not defined.

  a = a ^ b;
  b = a ^ b;
  a = a ^ b ;
  printf ("Safe 3 liner XOR a = %d, b = %d\n", a, b);
  
  a=(b=(a=b^a)^b)^a;
  printf ("a = %d, b = %d\n", a, b);
  
  a^=b^=a^=b;
  printf ("a = %d, b = %d\n", a, b);
  
  a+=b-=a=b-a;
  printf ("a = %d, b = %d\n", a, b);

  b=a+b-(a=b);
  printf ("a = %d, b = %d\n", a, b);

  if ((a!=0) && (b!=0))
  {
    a/=b=(a=a*b)/b;
    printf ("a = %d, b = %d\n", a, b);
  }
  else 
    printf("Either a or b is zero, skipping the oneliner with division\n");

  return 0;
}

