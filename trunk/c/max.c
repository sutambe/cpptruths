#include <stdio.h>

int main (int argc, char *argv[])
{
	int a = atoi (argv[1]);
        int b = atoi (argv[2]);
	printf("%d\n",((a+b)+abs(a-b))/2);
	return 0;
}
