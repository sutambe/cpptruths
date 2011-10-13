#include <stdio.h>

typedef int (*OUTPUT_FPTR)(int, size_t, double);
typedef int (*FPTR)(...);
typedef int (*NOTHING_FPTR)();

int oprintf(OUTPUT_FPTR, const char *)
{
  printf("OUTPUT_FPTR\n");
  return 0;
}

int oprintf(FPTR, const char *)
{
  printf("FPTR\n");
  return 0;
}

int oprintf(NOTHING_FPTR, const char *)
{
  printf("NOTHING_FPTR\n");
  return 0;
}

int func(int, size_t, double) 
{
  return 999;  
}

int func_many(...) 
{
  return 0;
}

int main (void)
{
  oprintf(func,"test");
  oprintf(func_many,"test");
  oprintf(main,"test");
  return 0;
}
