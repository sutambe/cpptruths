#include <stdio.h>
int main(c){ return putchar((c=getchar())^' '),!~c?:main();}
/*
char func(void)
{
char c;
c=getchar();
if ((int)c!=10)
return c;
else
return 0;
}
*/
