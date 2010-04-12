#include <stdio.h>

void myfunc(int);
int main(void)
{
	int i,j;
	i=0;
	j=sizeof(i? (double)0 : 'c');
	printf("i = %d, j = %d\n",i,j);

	return 0;
}

void myfunc(int n)
{
	printf("myfunc, n = %d.\n");
	return;// (double)n;
}
