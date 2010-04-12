  #include "integer.h"

  int *integer_1_svc(int *intptr,  struct svc_req *X)
  {
      printf("Function called with *intptr=%d\n",*intptr);
      (*intptr)++;
      return intptr;
  }

