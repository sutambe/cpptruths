#include <stdio.h>

#define printf(fmt, ...)

/*
gcc disable_printf.c -o disable_printf -ansi -pedantic -Wall
*/

int main (void)
{
  printf ("Sumant\n");
  return 0;
}

