#include <cstdio>
#include <cstdlib>

static bool IsPrime(size_t number) 
{    
  if (number <= 1) 
    return false;

  for (size_t i = 2; i*i <= number; ++i)        
    if (number % i == 0) 
      return false;    
  
  return true;
}

int main(int argc, char * argv[])
{
  if(argc < 2) {
    printf("nth-prime <N>\n");
    return 0;
  }

  unsigned int count = 0;
  unsigned int N = atoi(argv[1]);
  
  if(!N) return 0;

  for(int i = 2; ;++i) 
  {
    if(IsPrime(i) && (++count == N)) 
    {
      printf("%d\n", i);
      break;
    }
  }

  return 0;
}
