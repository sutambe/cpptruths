#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// When using condition variables there is always a Boolean predicate involving
// shared variables associated with each condition wait that is true if the
// thread should proceed. Spurious wakeups from the pthread_cond_timedwait() or
// pthread_cond_wait() functions may occur. Since the return from
// pthread_cond_timedwait() or pthread_cond_wait() does not imply anything about
// the value of this predicate, the predicate should be re-evaluated upon such
// return

typedef enum { Even, Odd, None } Turn;
int count = 0;
Turn turn = None;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;

void *even (void *args)
{
  while(1)
  {
    pthread_mutex_lock(&lock);
    while(turn != Even)
      pthread_cond_wait(&condition, &lock);
    printf("even %d\n", count++);
    turn = Odd;
    pthread_cond_broadcast(&condition);
    pthread_mutex_unlock(&lock);
  }
}
void *odd (void *args)
{
  while(1)
  {
    pthread_mutex_lock(&lock);
    while(turn != Odd)
      pthread_cond_wait(&condition, &lock);
    printf("odd %d\n", count++);
    turn = Even;
    pthread_cond_broadcast(&condition);
    pthread_mutex_unlock(&lock);
  }
}

int main ()
{
  char buf[32];

  pthread_t t1,t2;
  pthread_create(&t2, NULL, odd, NULL);
  pthread_create(&t1, NULL, even, NULL);
  sleep(1);
  
  pthread_mutex_lock(&lock);
  printf("Start\n");
  sleep(1);
  turn = Even;
  pthread_cond_broadcast(&condition);
  pthread_mutex_unlock(&lock);
  
  printf("Enter \"end\" to end...\n");
  while(strcmp(gets(buf), "end") != 0);

  return 0;
}
