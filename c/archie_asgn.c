#include <stdio.h>

main()
{
 int **x;
 int *y;
 int z;

x= (int**) malloc(sizeof(int*)); 
y= (int*) malloc(sizeof(int)); 

z= 1;
*y=2;
*x=y;
**x=z;
printf("%d\n",*y);
z=3;
printf("%d\n",*y);
**x =4;
printf("%d\n",z);
return 0;

}

