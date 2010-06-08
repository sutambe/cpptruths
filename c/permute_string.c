#include <stdio.h>
#include <string.h>

void swap(char *a, char *b)
{
  char temp = *a;
  *a = *b;
  *b = temp;
}

void permute(char *str, const int begin, const int end) 
{
  int i = begin;
  int j;
  
  for (i = begin; i < end-1; ++i) 
  {
    for (j = i+1; j < end; ++j) 
    {
       swap(str + i, str + j);
       permute(str, i+1, end);
       swap(str + i, str + j);
    }
  }
  printf("%s\n", str);
}

int main() 
{
  char str[] = "1234";
  permute(str, 0, strlen(str));
  return 0;
}
