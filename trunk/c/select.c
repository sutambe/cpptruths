#include <stdio.h>
#include <string.h> //for memset
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#define STDIN 0 /* file descriptor for standard input */  

int main(void) 
{ 
  struct timeval tv; //a structure used to represent time; it has seconds and usec components

  fd_set readfds;  //set of file descriptors

  tv.tv_sec = 1; 
  tv.tv_usec = 500000; 

  FD_ZERO(&readfds); //zero out the set of descriptors we are listening to

  FD_SET(STDIN, &readfds); // add STDIN to the set of descriptors we are listening to

   select(STDIN+1, &readfds, NULL, NULL, &tv); 
   if (FD_ISSET(STDIN, &readfds))
      printf("A key was pressed!\n");
    else printf("Timed out.\n"); 

  return 0;
} 

