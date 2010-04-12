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

void *start_thread(void *thread_id) 
{
     int me,next,*ptrid, iter = 1;
     ptrid =(int *)thread_id;
     me = *ptrid;
       
       while(1)
       {
          next = (me+1)%T;
          while(turn!= me);
          v_counter++;
          printf("THREAD %d in ITERATION %d ,COUNTER VALUE SET TO %d\n",me,iter,v_counter);
          turn = next;
          if((++iter)>N)
          {
            printf("EXIT THREAD:: %d\n",me);
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
          printf("Thread  couldnt be created ....\n");
         }
	
          for (i = 0; i < T; ++i) 
  	  pthread_join(threads[i], NULL);
     return 0;
}
