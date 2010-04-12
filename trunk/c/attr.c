#include <stdio.h>

typedef struct { char a[16]; } size16;

int main(void)
{
	size16 a __attribute__((aligned(16)));
	char c;// __attribute__((aligned(16)));

	int b;

	printf("a=%i c=%i &c=%p &a=%p\n",__alignof__(a),__alignof__(c),&c,&a);

}
