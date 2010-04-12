#include "rpc/rpc.h"
#include "integer.h"
incr_out *
incrproc_1_svc(incr_in *inp,  struct svc_req *rqstp)
  {
   static incr_out out;
   //printf("Connected...");
   sleep(5);
   out.res1 = inp-> arg1 + 1;
   return(&out);
  //printf("Value passed is integer \n");
  }

