#include <stdio.h>
void func ();
int main (void)
{
  int i, j;
  float f;
  func (i,j,f);

  return 0;
}
void func ()
{
  printf ("Called func\n");
}

