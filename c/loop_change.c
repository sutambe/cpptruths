#include <stdio.h>

int main (void)
{
  int i, n;
  /* Add or replace one character in this loop so that it terminates 
  for (i = 0, n = 20; i < n; i--)
  {
    printf("X");
  } 
  */
  for (i = 0, n = 20; -i < n; i--)
  {
    printf("X");
  } 
  printf ("\n");
  for (i = 0, n = 20; i < n; n--)
  {
    printf("X");
  } 
  printf ("\n");
  for (i = 0, n = 20; i + n; i--)
  {
    printf("X");
  } 
  printf ("\n");

  return 0;
}

