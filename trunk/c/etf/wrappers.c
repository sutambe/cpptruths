#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <tcl.h>

extern int do_redirect;
extern void _dl_ignore_next(char* name);
extern Tcl_Interp* TclInterp;


int myputchar (int c)
{
  Tcl_Obj *cmdvector[3];
  int retval;
  do_redirect = 0;
  cmdvector[0] = Tcl_NewStringObj("myputchar_begin",-1);
  cmdvector[1] = Tcl_NewIntObj(c);
  Tcl_EvalObjv(TclInterp,2,cmdvector,TCL_EVAL_GLOBAL);
  _dl_ignore_next("putchar");
  do_redirect = 1;
  retval = putchar (c);
  do_redirect = 0;
  _dl_ignore_next("");
  cmdvector[0] = Tcl_NewStringObj("myputchar_end",-1);
  cmdvector[1] = Tcl_NewIntObj(retval);
  cmdvector[2] = Tcl_NewIntObj(c);
  Tcl_EvalObjv(TclInterp,3,cmdvector,TCL_EVAL_GLOBAL);
  do_redirect = 1;
  return retval;
}

