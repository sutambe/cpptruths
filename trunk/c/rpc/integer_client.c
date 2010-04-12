  #include <rpc/rpc.h>
  #include "integer.h"

  main()
  {
      CLIENT *pclnt;
      int number=100;
      int   *intptr=&number;
    
      char    Host[256];

      /* Get the name of the host running the server. */
    
      gethostname(Host, 256);

      /* Connect to the server. */
    
      pclnt = clnt_create(Host, MJLPROG, MJLVERS, "udp");

      /* Issue a request to the server. */
    
      intptr=integer_1(intptr, pclnt);

      printf("Client: *intptr=%d\n",*intptr);

      /* Disconnect from the server. */
    
      clnt_destroy(pclnt);

  }
