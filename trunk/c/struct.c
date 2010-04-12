#include <stdio.h> 

struct A 
{ 
	int x; 
	int y; 
}; 
struct B 
{ 
	struct A a; 
	double b; 
}; 
struct B obj = { .b = 12.34,
		 .a = { .y = 47, .x = 37 }
		  };
union PQR
{
char ch;
int i[];
}; 
int main() 
{ 
	
	union PQR z[20];
	struct A data[] = { [100].x = 37, [100].y = 47 };
	printf("%d %d %g\n", obj.a.x, obj.a.y, obj.b); 
	printf("sizeof z = %d\n",sizeof(z));
	return 0; 
}
