#include <stdio.h>
#include <math.h>
#include <sys/time.h>

#define MyTime timeval

#if defined MyTime == timeval
#  define SEC tv_sec
#  define FRAC_SEC tv_usec
#  define SCALE 1000000
#else 
#  undef  MyTime
#  define MyTime timespec
#  define SEC tv_sec
#  define FRAC_SEC tv_nsec
#  define SCALE 1000000000
#endif

struct MyTime getCurrentMyTime(struct MyTime * t)
{
  struct timeval tv;  
  gettimeofday(&tv, NULL);
  t->SEC = tv.tv_sec;
#if defined MyTime == timeval
  t->FRAC_SEC = tv.tv_usec;
#else  
  t->FRAC_SEC = tv.tv_usec * 1000;
#endif
  return *t;
}

void print_time (const char *str, struct MyTime t)
{
  printf("%s sec = %ld, fraction_sec = %ld\n",str, t.SEC, t.FRAC_SEC);
}

int Equal (struct MyTime t1, struct MyTime t2)
{
  if ((t1.SEC == t2.SEC) &&
      (t1.FRAC_SEC == t2.FRAC_SEC))
    return 1;
  else
    return 0;
}

int LessThan (struct MyTime t1, struct MyTime t2)
{
  if (t1.SEC < t2.SEC)
    return 1;
  else if (t1.SEC > t2.SEC)
    return 0;
  else 
    return t1.FRAC_SEC < t2.FRAC_SEC;
}

struct MyTime Min (struct MyTime t1, struct MyTime t2)
{
  if (LessThan (t1, t2))
    return t1;
  else
    return t2;
}

struct MyTime Max (struct MyTime t1, struct MyTime t2)
{
  if (LessThan (t1, t2))
    return t2;
  else
    return t1;
}

struct MyTime Diff (struct MyTime t1, struct MyTime t2)
{
  struct MyTime min = Min(t1, t2);
  struct MyTime max = Max(t1, t2);
  struct MyTime diff = { 0, 0 };

  if (Equal(min,max))
    return diff;
  
  if (max.FRAC_SEC < min.FRAC_SEC)
  {
    max.FRAC_SEC += SCALE;
    max.SEC -= 1;
  }
  
  diff.FRAC_SEC = max.FRAC_SEC - min.FRAC_SEC;
  diff.SEC = max.SEC - min.SEC;
  return diff;
}

struct MyTime Avg (struct MyTime t1, int count)
{
  struct MyTime avg = { t1.SEC / count, t1.FRAC_SEC / count };
  return avg;

//  struct MyTime avg = { 0, 0 };
//  long long nanosec = t1.SEC * SCALE + t1.FRAC_SEC;
//  nanosec /= count;
//  avg.SEC = nanosec / SCALE;
//  avg.FRAC_SEC = nanosec % SCALE;
//  return avg;
}

int main (void)
{
  struct MyTime first_time = { 10, 501};
  struct MyTime second_time = { 2, 500};
  struct MyTime min_time = { 99999, 999999};
  struct MyTime max_time = { 0, 0};
  struct MyTime diff_time = { 0, 0};
  
  print_time ("First:", first_time);
  print_time ("Second:", second_time);

  print_time ("Min:", Min(first_time,second_time));
  print_time ("Max:", Max(first_time,second_time));
  print_time ("Diff:", Diff(second_time, first_time));
  
  getCurrentMyTime(& first_time);
  sleep (10);
  getCurrentMyTime(& second_time);
  
  print_time ("Average:", Avg(Diff(second_time,first_time), 10));
  
   diff_time.SEC = first_time.SEC-second_time.SEC;
   diff_time.FRAC_SEC = first_time.FRAC_SEC-second_time.FRAC_SEC;
    
  /* If the substraction results in negative answer */

   if(diff_time.FRAC_SEC < 0)
    {
        diff_time.FRAC_SEC = SCALE + diff_time.FRAC_SEC;
            if(diff_time.SEC > 0 )
               diff_time.SEC = diff_time.SEC-1;
            else
               diff_time.FRAC_SEC*=-1;
    }

   if(diff_time.FRAC_SEC < min_time.FRAC_SEC)
    min_time.FRAC_SEC = diff_time.FRAC_SEC;
  
   if(diff_time.FRAC_SEC > max_time.FRAC_SEC)
    max_time.FRAC_SEC = diff_time.FRAC_SEC;

   if(diff_time.SEC < min_time.SEC)
    min_time.SEC = diff_time.SEC;
  
   if(diff_time.SEC > max_time.SEC)
    max_time.SEC = diff_time.SEC;

   print_time("Old Min:", min_time);
   print_time("Old Max:", max_time);

  return 0;
}
