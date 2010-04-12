#include <setjmp.h>
#include <stdio.h>

/*
Compile using optimization option and see the effect of
volatile. Optimizations are evil.
*/

static jmp_buf  buf;

void func ()
{
  longjmp (buf, 1);
}

int main()
{
    int b;
    b=3;
    b=7;

    if(setjmp(buf)!=0)  
    { 
       printf("%d\n", b);  
       return 0;
    }
    b=5;
    printf ("sizeof (jmp_buf) = %d\n",sizeof(jmp_buf));
    func();
    
    return 0;
}
/*
volatile variable isn't affected by the optimization. Its value after the
longjump is the last value variable assumed. 
   
Transfers control to the statement where the call to setjmp (which initialized
buf) was made. Execution continues at this point as if longjmp cannot return
the value 0.A nonvolatile automatic variable might be changed by a call to
longjmp.When you use setjmp and longjmp, the only automatic variables
guaranteed to remain valid are those declared volatile.
*/
