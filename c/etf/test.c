#include <unistd.h>
#include <stdio.h>
#include <string.h>
//#include <pthread.h>
#include <stdlib.h>

char *MYstrrchr(const char *s, int c);
unsigned int (*s)(unsigned int seconds);
void *test(void *);
int main(int argc, char **argv)
{
   int i = {0};
   char *chptr=0;
   char command[100]={0};
   //pthread_t testthread,t2;
   s=sleep;
   chptr=strrchr("nmsu",'m');
   //pthread_create(&testthread,NULL,test,NULL);
   //pthread_create(&t2,NULL,test,NULL);
   //fprintf(stdout,"environ=%p\n",environ);
   chptr=malloc(10);
   chptr=malloc(10);
   for(i=0;i < 2;++i)
   {
      putchar('A');
      //printf("Type 'quit' to exit: \n");
      fflush(stdout);
      //scanf("%s",command);
      if(0==strcmp(command,"quit"))
      {
          break;
      }
      (*s)(1);
      /*if(s==sleep)
      {
          printf("It is same\n");
      }
      else printf("It is not same\n");*/
      //fflush(stdout);
      chptr=strrchr("CS",'S');
   }
   //MYstrrchr("India",'d');*/
   //pthread_join(testthread,0);
   //pthread_join(t2,0);
   printf("This is test %p\n",chptr);
   //fflush(stdout);
   chptr=strrchr("PLEASE",'E');
   fprintf(stdout,"Sumant\n");
   exit(i);
}
char *MYstrrchr(const char *s, int c)
{
    printf("MYstrrchr(main): Searching %s for %c.\n",s,c);
    return strrchr(s,c);
}
void *test(void *arg)
{
    int i;
    for(i=0;i < 1;i++)
    {
        //fprintf(stdout,"Test Thread\n");
        //fflush(stdout);
    }
    return 0;
}

