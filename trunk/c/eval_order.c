#include <stdio.h>
static int a=3;

static int f()
{
return a=1,2;
}

int main(void)
{
printf("%d %d\n",f(),a);
return 0;
}
