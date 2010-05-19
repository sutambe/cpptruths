#include <stdio.h>
#include <string.h>
#include <malloc.h>

extern void redirect_ignore_next(char *fname, char *lname);
extern int invoke_tcl_proc(char *procname, char *fmt, ...);
extern int do_redirect;

int wrap_printf (char* format, double d1,double d2,double d3,double d4,double d5, 
                 double d6, double d7, double d8, double d9,double d10)
{
    int retval;
    static int entry=0;
    do_redirect=0;
    puts("WRAP_PRINTF");
    if(invoke_tcl_proc("wrap_printf_begin","%s %d",format,entry++))
    {
        fprintf(stderr,"ERROR\n");
    }
    printf("came here\n");
    retval=printf(format,d1,d2,d3,d4,d5,d6,d7,d8,d9,d10);
    invoke_tcl_proc("wrap_printf_end","%d %s",100,format);
    entry=0;
    do_redirect=1;
    return retval;
}

void *wrap_malloc(size_t size)
{
    void *retval;
    static int ignnext_called=0;
    int temp_do_redirect=do_redirect;
    
    do_redirect=0;
    //puts("WRAP_MALLOC");
    //invoke_tcl_proc("wrap_malloc_begin","%d",size);
    if(0 == ignnext_called)
    {
        redirect_ignore_next("malloc","libwrappers3.so");
        ignnext_called=1;
    }   
    do_redirect=temp_do_redirect;
    retval=malloc(size);
    do_redirect=0;
    redirect_ignore_next("",""); /* Not actually required */
    invoke_tcl_proc("wrap_malloc_end","%d%d",retval,size);
    do_redirect=temp_do_redirect;
    return retval;
}
