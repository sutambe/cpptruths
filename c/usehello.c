#include <stdio.h>
#include <dlfcn.h>
#include "hello.h"

int main(void)
{
    void *libhandle;
    void (*uhello)(int);

    libhandle=dlopen("libhello.so",RTLD_LAZY);
    uhello=dlsym(libhandle,"usehello");

    (*uhello)(5);

    dlclose(libhandle);
    printf("CALLING DIRECTLY");
    usehello(5);
    return 0;
}
