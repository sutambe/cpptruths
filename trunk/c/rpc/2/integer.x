/* RPCGEN code decribing the client/server API. */
 struct incr_in{
        int arg1;
 };

struct incr_out{
       int res1;
};
  program INCR_PROG
  {
    version INCR_VERS
    {
      incr_out  INCRPROC(incr_in) = 1;

    } = 1;
  } = 0x31230000;

