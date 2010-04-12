#include <string.h>
#include <stdio.h>

int main(void)
{
	int a=10,b=20,c=30;
	int val;

	//val=a?b:(c=0)=10;
	val=a?:b;
	printf("%d %u",*"",strlen(""));

	printf("val = %d, c= %d, b= %d\n",val,c,b);

	return 0;
}
