#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<linux/unistd.h>
#include<time.h>
#include<errno.h>

int N ,T ;
int v_counter = 0;
int turn;

pthread_mutex_t c_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c_threshold_cv = PTHREAD_COND_INITIALIZER ;


void *start_thread(void *thread_id) 
{
     int me,next,*ptrid, iter = 1;
     ptrid =(int *)thread_id;
     me = *ptrid;
       
       while(1)
       {
          next = (me+1)%T;
          pthread_mutex_lock(&c_mutex);
          while(turn!= me)
          {
              pthread_cond_wait(&c_threshold_cv,&c_mutex);
          }
          v_counter++;
          printf("\nTHREAD %d in ITERATION %d ,COUNTER VALUE SET TO %d",me,iter,v_counter);
          pthread_mutex_unlock(&c_mutex);
          turn = next;
          pthread_cond_broadcast(&c_threshold_cv);

          if((++iter) > N)
          {
           printf("\n EXIT THREAD:: %d\n",me);
           pthread_exit(NULL);
          } 
            
       } 
}


int main(int argc, char *argv[])
{
  	int i = 0,status = 0;
 
        N = atoi(argv[1]) ; 
        T = atoi(argv[2]) ;

  	pthread_t threads[T];
        int thread_ids[T]; 
   
         for (i = 0; i < T; i++) 
         {
	          thread_ids[i] = i ;    
	          status = pthread_create(&threads[i], NULL, start_thread,&thread_ids[i]);
	          /* Error handling */
	          if(status)
	          printf("\nThread  couldnt be created ....");
         }
	
          for (i = 0; i < T; ++i) 
  	  pthread_join(threads[i], NULL);

          return 0;
}
