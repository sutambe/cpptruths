#include <stdio.h>

typedef int (*OUTPUT_FPTR)(int, size_t, double);
typedef int (*FPTR)(...);
typedef int (*NOTHING_FPTR)();

extern "C" { // Turns off C++ mode so overloading is not allowed.

extern int oprintf(OUTPUT_FPTR, const char *i, ...);
extern int oprintf(FPTR, const char *, ...) __attribute__((deprecated));

extern int oprintf(FPTR, const char *, ...) 
{
  printf("FPTR\n");
  return 0;
}

extern int oprintf(NOTHING_FPTR, const char *, ...);

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

extern int oprintf(OUTPUT_FPTR, const char *i, ...)
{
  printf("OUTPUT_FPTR\n");
  return 0;
}

extern int oprintf(NOTHING_FPTR, const char *, ...)
{
  printf("NOTHING_FPTR\n");
  return 0;
}

