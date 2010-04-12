#include <iostream>

//#define  VAL 0xFFFFFFFE
#define  VAL ~c

int main (void)
{
  char c = 1; 
  unsigned long long int i = VAL;
  long long int l = VAL;
  unsigned int m = VAL;
  std::cout << i << std::endl;
  std::cout << l << std::endl;
  std::cout << m << std::endl;
  std::cout << sizeof(long int) << std::endl;
}
