#include <stdio.h>
#include <stdlib.h>

unsigned long fib_helper(unsigned n, long a, long b)
{
  return n? fib_helper(--n, b, a+b) : a;
}

unsigned long fib_fast (unsigned n)
{
  return fib_helper(n,0,1);
};

unsigned long fibonacci(unsigned n)
{
  return (n < 2) ? n : fibonacci(n - 1) + fibonacci(n - 2);
}

int main(int argc, char *argv[])
{
  int n = atoi(argv[1]);
  printf("%10ld\n",fib_fast(n));
  printf("%10ld\n",fibonacci(n));
  return 0;
}
