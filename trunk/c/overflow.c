#include<stdio.h>
int main()
{
//int i=0xFFFFFF00;
//for( ;i++; );/*semicolon required*/
//printf("%d",i);
  unsigned long i = (unsigned) 70000l * (unsigned) 70000l / (unsigned) 70000l;
  cout << i << std::endl;
  return 0;
}
