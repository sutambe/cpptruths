#include <stdio.h>
#include <string.h>

int compare (char c1, char c2)
{
  printf("%c==%c\n", c1, c2);
  return c1==c2;
}

int less (char c1, char c2)
{
  printf("%c < %c\n", c1, c2);
  return c1 < c2;
}

int bsearch(char *a, char c, size_t n)
{
  if (n==1)
    return compare(a[0],c);
  
  if(compare(a[0],c))
    return 1;
  else if(compare(a[n-1],c))
    return 1;
  
  size_t temp = n/2;
  if(compare(a[temp],c))
    return 1;
  if (less(c,a[temp]))
    return bsearch(a, c, temp);
  else
    return bsearch(&a[temp+1], c, n-temp-1);
}

int main(void)
{
  char array[] = "abcdefghijklmnopqrstuvwxyz";
  printf("%d", bsearch(array, 'i', strlen(array)));
}
