#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void swap(char * a, char * b)
{
  char temp = *a;
  *a = *b;
  *b = temp;
}

int main(int argc, char * argv[])
{
  assert(argc == 3);
  int K = atoi(argv[2]);
  char * array = argv[1];

  int i =0;
  for(i = K; array[i]; ++i)
  {
    swap(array + i, array + i - K);
  }

  printf("%s\n", array);
  return 0;
}
