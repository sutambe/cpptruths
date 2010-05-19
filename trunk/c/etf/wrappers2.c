#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <dlfcn.h>
#include <tcl.h>
#include <unistd.h>

#include "redirect.h"

extern int do_redirect;
extern Tcl_Interp* TclInterp;

void wrap_printf (char* format, double d1,double d2,double d3,double d4,double d5);
void* wrap_malloc (size_t numbytes);
void wrap_free (void* ptr);
void myputchar (int c);
void mydllmain (int a, int b, char * c);
char *mystrrchr(const char *s, int c);
void *mydlopen(const char *filename, int flag);
void *mydlsym(void *handle, char *symbol);
unsigned int mysleep(unsigned int seconds);
int dllmain (int, int, char *);
int mystrcmp(char *s1, char *s2);

int mystrcmp(char *s1, char *s2)
{
  int retval=0;
  do_redirect = 0;
  printf("mystrcmp %p %p\n",s1,s2);
  retval = strcmp(s1,s2);
  do_redirect = 1;
  return retval;
}

void wrap_printf (char* format, double d1,double d2,double d3,double d4,double d5)
{
  Tcl_Obj *cmdvector[4];
  do_redirect = 0;
  cmdvector[0] = Tcl_NewStringObj("wrap_printf_begin",-1);
  cmdvector[1] = Tcl_NewStringObj(format,-1);
  Tcl_EvalObjv(TclInterp,2,cmdvector,TCL_EVAL_GLOBAL);
  redirect_ignore_next("printf","/home/trutta2/srg/projects/dynamic-loader/sumant-tambe/redirect-lib/libwrap2.so.0.0");
  do_redirect = 1;
  printf (format, d1,d2,d3,d4,d5);
  do_redirect = 0;
  redirect_ignore_next("","");
  cmdvector[0] = Tcl_NewStringObj("wrap_printf_end",-1);
  cmdvector[1] = Tcl_NewStringObj("void",-1);
  cmdvector[2] = Tcl_NewStringObj(format,-1);
  Tcl_EvalObjv(TclInterp,3,cmdvector,TCL_EVAL_GLOBAL);
  do_redirect = 1;
  return;
}

void* wrap_malloc (size_t numbytes)
{
  Tcl_Obj *cmdvector[3];
  void* retval;
  do_redirect = 0;
  cmdvector[0] = Tcl_NewStringObj("wrap_malloc_begin",-1);
  cmdvector[1] = Tcl_NewLongObj(numbytes);
  Tcl_EvalObjv(TclInterp,2,cmdvector,TCL_EVAL_GLOBAL);
  redirect_ignore_next("malloc","/home/trutta2/srg/projects/dynamic-loader/sumant-tambe/redirect-lib/libwrap2.so.0.0");
  do_redirect = 1;
  retval = malloc (numbytes);
  do_redirect = 0;
  redirect_ignore_next("","");
  cmdvector[0] = Tcl_NewStringObj("wrap_malloc_end",-1);
  cmdvector[1] = Tcl_NewLongObj((unsigned)retval);
  cmdvector[2] = Tcl_NewLongObj(numbytes);
  Tcl_EvalObjv(TclInterp,3,cmdvector,TCL_EVAL_GLOBAL);
  do_redirect = 1;
  return retval;
}

void wrap_free (void* ptr)
{
  Tcl_Obj *cmdvector[3];
  do_redirect = 0;
  cmdvector[0] = Tcl_NewStringObj("wrap_free_begin",-1);
  cmdvector[1] = Tcl_NewLongObj((unsigned)ptr);
  Tcl_EvalObjv(TclInterp,2,cmdvector,TCL_EVAL_GLOBAL);
  redirect_ignore_next("free","/home/trutta2/srg/projects/dynamic-loader/sumant-tambe/redirect-lib/libwrap2.so.0.0");
  do_redirect = 1;
  free (ptr);
  do_redirect = 0;
  redirect_ignore_next("","");
  cmdvector[0] = Tcl_NewStringObj("wrap_free_end",-1);
  cmdvector[1] = Tcl_NewStringObj("void",-1);
  cmdvector[2] = Tcl_NewLongObj((unsigned)ptr);
  Tcl_EvalObjv(TclInterp,3,cmdvector,TCL_EVAL_GLOBAL);
  do_redirect = 1;
  return;
}

void myputchar (int c)
{
  Tcl_Obj *cmdvector[3];
  do_redirect = 0;
  cmdvector[0] = Tcl_NewStringObj("myputchar_begin",-1);
  cmdvector[1] = Tcl_NewIntObj(c);
  Tcl_EvalObjv(TclInterp,2,cmdvector,TCL_EVAL_GLOBAL);
  redirect_ignore_next("putchar","/home/trutta2/srg/projects/dynamic-loader/sumant-tambe/redirect-lib/libwrap2.so.0.0");
  do_redirect = 1;
  putchar (c);
  do_redirect = 0;
  redirect_ignore_next("","");
  cmdvector[0] = Tcl_NewStringObj("myputchar_end",-1);
  cmdvector[1] = Tcl_NewStringObj("void",-1);
  cmdvector[2] = Tcl_NewIntObj(c);
  Tcl_EvalObjv(TclInterp,3,cmdvector,TCL_EVAL_GLOBAL);
  do_redirect = 1;
  return;
}
/*
void mydllmain (int a, int b, char * c)
{
  Tcl_Obj *cmdvector[5];
  do_redirect = 0;
  cmdvector[0] = Tcl_NewStringObj("mydllmain_begin",-1);
  cmdvector[1] = Tcl_NewIntObj(a);
  cmdvector[2] = Tcl_NewIntObj(b);
  cmdvector[3] = Tcl_NewStringObj(c,-1);
  Tcl_EvalObjv(TclInterp,4,cmdvector,TCL_EVAL_GLOBAL);
  redirect_ignore_next("dllmain");
  do_redirect = 1;
  dllmain (a, b, c);
  do_redirect = 0;
  redirect_ignore_next("");
  cmdvector[0] = Tcl_NewStringObj("mydllmain_end",-1);
  cmdvector[1] = Tcl_NewStringObj("void",-1);
  cmdvector[2] = Tcl_NewIntObj(a);
  cmdvector[3] = Tcl_NewIntObj(b);
  cmdvector[4] = Tcl_NewStringObj(c,-1);
  Tcl_EvalObjv(TclInterp,5,cmdvector,TCL_EVAL_GLOBAL);
  do_redirect = 1;
  return;
}
*/
char *mystrrchr(const char *s, int c)
{
    char *cptr;
    printf("mystrrchr2: Searching %s for %c.\n",s,c);
    redirect_ignore_next("strrchr","/home/trutta2/srg/projects/dynamic-loader/sumant-tambe/redirect-lib/libwrap2.so.0.0");
    cptr=strrchr(s,c);
    redirect_ignore_next("","");
    return cptr;
}

void *mydlopen(const char *filename, int flag)
{
    void *handle;
    printf("dlopen: %s %X\n",filename,flag);
    redirect_ignore_next("dlopen","/home/trutta2/srg/projects/dynamic-loader/sumant-tambe/redirect-lib/libwrap2.so.0.0");
    handle=dlopen(filename,flag);
    redirect_ignore_next("","");
    return handle;
}

void *mydlsym(void *handle, char *symbol)
{
    void *h;
    printf("dlsym: %s\n",symbol);
    redirect_ignore_next("dlsym","/home/trutta2/srg/projects/dynamic-loader/sumant-tambe/redirect-lib/libwrap2.so.0.0");
    h=dlsym(handle,symbol);
    redirect_ignore_next("","");
    return h;
}

unsigned int mysleep(unsigned int seconds)
{
    unsigned int s;
    printf("mysleep: %d\n",seconds);
    redirect_ignore_next("sleep","libwrap2.so.0.0");
    s=sleep(seconds);
    redirect_ignore_next("","");
    return s;
}

void _fini(void)
{
    //printf("_fini has been called for wrappers.c\n");
}

