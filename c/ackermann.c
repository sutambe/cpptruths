#include <stdio.h>
#include <stdlib.h>

#define PARTIALLY_ITERATIVE TRUE

long int A(int,int);
int main(int argc, char *argv[])
{
    int i,j;
    int m=atoi(argv[1]);
    int n=atoi(argv[2]);
    
    
    for(i=0;i <= m; i++)
        for(j=0;j <= n; j++)
            printf("A(%d,%d)=%ld\n",i,j,A(i,j));
          
    printf("A(%d,%d)=%ld\n",m,n,A(m,n));
    
    return 0;
}
#ifndef PARTIALLY_ITERATIVE
long int A(int m,int n)
{
    if(0==m && 0==n)
        return 1;
    else if(0==m)
        return n+1;
    else if(0==n)
        return A(m-1,1);
    else 
        return A(m-1,A(m,n-1));
}
#endif
#ifdef PARTIALLY_ITERATIVE
long int A(int m, int n)
{
     while (m != 0)
     {
         n=n ? A(m, n-1) : 1;
         --m;
     }
     return n+1;
}
#endif
/*
 Ackermann's function

    A(0, n) = n + 1
    A(m+1, 0) = A(m, 1)
    A(m+1, n+1) = A(m, A(m+1, n))

A(m,n) 	n = 0 	n = 1 	n = 2 	n = 3  	n = 4 	n = 5
m = 0 	1     	2     	3     	4     	5     	6
m = 1 	2     	3     	4     	5     	6     	7
m = 2 	3     	5     	7     	9     	11     	13
m = 3 	5     	13     	29     	61     	125 	253
m = 4 	13    	65533  	265536-3 	2265536-3 	A(3,2265536-3) 	A(3,A(4,4))
m = 5 	65533 	A(4,65533) 	A(4,A(4,65533)) 	A(4,A(5,2)) 	A(4,A(5,3)) 	A(4,A(5,4))
m = 6 	A(4,65533) 	A(5,A(4,65533)) 	A(5,A(6,1)) 	A(5,A(6,2)) 	A(5,A(6,3)) 	A(5,A(6,4))

Ackermann's function, while Turing computable, grows faster than any primitive recursive function. The reason can be seen from the table: the index for the next value is also growing.

An equivalent defininition is:

    A(0, n) = n + 1
    A(1, n) = 2 + (n + 3) - 3
    A(2, n) = 2 × (n + 3) - 3
    A(3, n) = 2 n + 3 - 3
    A(4, n) = 22...2 - 3        (n + 3 terms)
    ...

remember that 2222 = 2(2(22)) = 65536, and not ((22)2)2 = 22×2×2 = 256

Conventional mathematical notation breaks down at this point, and something more extensible is needed, invented specially to define big numbers.

    * Cutland. Computability, section 2.5.5
*/

