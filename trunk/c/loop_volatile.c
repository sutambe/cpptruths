#include <stdio.h>
#include <signal.h>

volatile int i = 1;

void sig (int s)
{
  i = 0;
}

int main(void)
{
  signal (SIGINT, sig);
  while (i);
  return 0;
}


