#include <stdio.h>

void bar (const int &i)
{
	//++i;
}

int main (void)
{
	int i=10, k=20;
	++++i = k;
	bar (i++);
	return 0;
}


