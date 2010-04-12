/**************************************
*	Please write you name here
*	NAME: 
*
*
* PROBLEM DESCRIPTION: 
* 	You are required to create three threads in the program  
*	given below. A varialbe v_counter is incremented by two threads (working threads)
*  The third thread is signaled by any of the two working threads
*	when the varialbe v_counter reaches a predefined value. 
*
*	NOTE
*		1: Some sample codes are given below. Feel free to change them or add your own
*			codes. 
*	 	2: Before you submit your program on the Blackboard, please make sure it can be 
*			compiled on some linux distributions, either Fedora or ubuntu. 
*		
******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
// what else you need? 

#define NUM_THREADS  3
// predefined limit for v_counter
#define COUNT_LIMIT 20
#define BUSY_WAIT_ENABLED 0


int     v_counter = 0;
int turn = 0;

pthread_mutex_t c_mutex;
pthread_cond_t c_threshold_cv;
void yield (void)
{
  // pthread_yield ();

  // It should give up the CPU somehow.
  // pthread library has a function called pthread_yield
  // but it is not found on my machine.
}

void busy_wait (int me, int next)
{
        while (1)
        {
          while (turn != me)
            yield(); // busy wait.
          
          if (v_counter >= COUNT_LIMIT)
          {
            turn = next;
            break;
          }
          
          v_counter++;
          printf ("T[%d] = %d.\n",me, v_counter);
          
          turn = next;
          if (v_counter >= COUNT_LIMIT)
            break;
        }
}

void no_busy_wait (int me, int next)
{
        while (1)
        {
          pthread_mutex_lock (&c_mutex); 
          while (turn != me)
            pthread_cond_wait (&c_threshold_cv, &c_mutex);

          if (v_counter == COUNT_LIMIT)
          {
            turn = next;
            pthread_mutex_unlock (&c_mutex);
            pthread_cond_signal (&c_threshold_cv);
            break;
          }
          else
          {
            v_counter++;
            printf ("T[%d] = %d.\n",me, v_counter);
            turn = next;
            pthread_mutex_unlock (&c_mutex);
            pthread_cond_signal (&c_threshold_cv);
          }
        }
}

void *working_thread(void *t) 
{
        int me = (int) t;
        int next = 0;
        if (me+1 == NUM_THREADS)
          next = 0;
        else
          next = me + 1;

        if (BUSY_WAIT_ENABLED)
          busy_wait(me, next);
        else
          no_busy_wait(me, next);
        /* here is your job */
	
	/*
		if v_counter == COUNT_LIMIT 
			send signal to the third thread. 
	*/
	
	/* Do some work so threads can alternate on mutex lock */
        printf ("Thread %d finished.\n",me);
        return 0;
}

// the test count thread
void *test_count(void *t) 
{
	/* here is your job */
        return 0;
}

int main(int argc, char *argv[])
{
  	int i = 0;
  	pthread_t threads[NUM_THREADS];
  	//pthread_attr_t attr[NUM_THREADS];

  	/* Initialize mutex and condition variable objects */
  	pthread_mutex_init(&c_mutex, NULL);
  	pthread_cond_init (&c_threshold_cv, NULL);

        for (i = 0; i < NUM_THREADS - 1; ++i) 
  	  pthread_create(&threads[i], NULL, working_thread, (void *)i);

  	pthread_create(&threads[i], NULL, working_thread, (void *)i);
	/*create other threads*/  
  
        for (i = 0; i < NUM_THREADS; ++i) 
  	  pthread_join(threads[i], NULL);

        return 0;
}
