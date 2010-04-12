#include <stdio.h>

//Disable printf everywhere.
#define printf(fmt, ...)

//#define DEBUG(format, ...) fprintf (stderr, format, __VA_ARGS__)
#define DEBUG(format, ...) fprintf (stderr, format, ##__VA_ARGS__)

int main (void)
{
  DEBUG("ABCD %d, %c\n",10, 'A');
  DEBUG("abcd\n");
  return 0;
}

/* gcc -Wall varargs.c -m varargs -std=c99 -pedantic
*/
