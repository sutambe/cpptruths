#include <rpc/rpc.h>
#include "integer.h"

 int  main(int argc,char **argv)
  {
      CLIENT *cl;
      incr_in  in;
      incr_out *outp;
      char Host[256];
      struct timeval tv;

     if(argc!=4)
     {
        fprintf(stderr,"usage: Client<hostname><integer-value><Protocol>\n");
        exit(1);
     }
          printf("\nConnected to Server.\n");

          gethostname(Host,256);

          cl = clnt_create(Host,INCR_PROG,INCR_VERS,argv[3]);
          clnt_control(cl,CLGET_TIMEOUT,(char *) &tv);
          printf("Timeout = %d sec,%d usec\n",tv.tv_sec,tv.tv_usec);
         in.arg1 = atol(argv[2]);
         if((outp = incrproc_1(&in,cl)) == NULL)
         printf("%s",clnt_sperror(cl,argv[1]));
         printf("-------------------------");
         printf("\nResult: %d", outp->res1);
         printf("\n-------------------------\n");
         exit(0);

}

