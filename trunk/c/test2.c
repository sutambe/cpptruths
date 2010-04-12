#include <stdio.h>

#define strfy(x) #x
#define concat(a,b) a##b

int main(void)
{
    printf(strfy(a\n)); 
    
    return 0;
}

