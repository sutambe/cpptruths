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

typedef enum { First, Second, Third, None } Turn;
Turn turn = None;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;

void *first (void *args)
{
  while(1)
  {
    pthread_mutex_lock(&lock);
    // Without the turn-while loop, the program blocks after some time.
    while(turn != First)
      pthread_cond_wait(&condition, &lock);
    printf("first\n");
    turn = Second;
    pthread_cond_broadcast(&condition);
    pthread_mutex_unlock(&lock);
  }
}
void *second (void *args)
{
  while(1)
  {
    pthread_mutex_lock(&lock);
    while(turn != Second)
      pthread_cond_wait(&condition, &lock);
    printf("second\n");
    turn = Third;
    pthread_cond_broadcast(&condition);
    pthread_mutex_unlock(&lock);
  }
}
void *third (void *args)
{
  while(1)
  {
    pthread_mutex_lock(&lock);
    while(turn != Third)
      pthread_cond_wait(&condition, &lock);
    printf("third\n");
    turn = First;
    pthread_cond_broadcast(&condition);
    pthread_mutex_unlock(&lock);
  }
}

int main ()
{
  char buf[32];

  pthread_t t1,t2,t3;
  pthread_create(&t3, NULL, third, NULL);
  pthread_create(&t2, NULL, second, NULL);
  pthread_create(&t1, NULL, first, NULL);
  sleep(1);
  
  pthread_mutex_lock(&lock);
  printf("Start\n");
  sleep(1);
  turn = First;
  pthread_cond_broadcast(&condition);
  pthread_mutex_unlock(&lock);
  
  printf("Enter \"end\" to end...\n");
  while(strcmp(gets(buf), "end") != 0);

  return 0;
}
