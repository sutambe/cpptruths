#include <stdio.h>

typedef struct { int p; double a[]; } large;

typedef void* (*loadfunc)(char *);
int myfunction(loadfunc c);
void *sample(char *);
void myfunc(int);
int main(void)
{
	int i,j;
	char array[0];/*={ '1', '2' };*/
	loadfunc func=sample;
	large l;
	i=j=1;
	j=sizeof(i? (double)1.0 : j? (float)2.0 : 'C');
	//printf("i = %d, j = %d\n",i,j);
	printf("%d\n",sizeof(large));
	l.a[1]=10;
	myfunction(func);	

	return 0;
}

void myfunc(int n)
{
	printf("myfunc, n = %d.\n");
	return;/* (double)n;*/
}
void *sample(char *c)
{
	return c;
}
int myfunction(loadfunc c)
{
	char pqr='A';
	(*c)(&pqr);
}

