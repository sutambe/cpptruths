# include <stdio.h>

int main (void)
{
  int i = 4, j = 0;
  // Comma has lower precedence than assignment.
  // Remember, you need a bracket for comma operator?
  i = i + 1 , ++i;

  printf ("%d\n",i);
  return 0;
}

