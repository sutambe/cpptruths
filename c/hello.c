
#define COMPILING_HELLO_C
#define static 

#include <stdio.h>

#define __USE_GNU TRUE
#include <dlfcn.h>
#include "hello.h"


static void _init(void)
{
    printf("_init: Hello\n");
}

static void _fini(void)
{
    printf("_fini: Goodbye\n");
}


void usehello(int n)
{
	Dl_info libinfo;
	int retval;

    //for(int i=0;i < n;++i)   printf("usehello: Hello\n");
	
	retval=dladdr(usehello,&libinfo);
	printf("dladdr returned %d\n",retval);
	if(retval)
	{
		printf("dli_fname=%s\n",libinfo.dli_fname); /* File name of defining object */
		printf("dli_fbase=%p\n",libinfo.dli_fbase); /* Load address of that object */
		printf("dli_sname=%s\n",libinfo.dli_sname); /* Name of nearest lower symbol */
		printf("dli_saddr=%p\n",libinfo.dli_saddr); /* Exact value of nearest symbol */
	}
}

