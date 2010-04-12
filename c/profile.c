#include <stdio.h>
#include <stdlib.h>

int fibo(int i);
int main(int argc,char **argv)
{
	int i=0,n;

	switch(argc)
	{
		case 1:
			fprintf(stderr,"Usage: %s <non -ve number>\n",argv[0]);
			break;
		case 2:
		{
			printf("Fibonacci series: ");
			switch(n=abs(atoi(argv[1])))
			{
				case 0:
					printf("\n");
					break;
				case 1:
					printf("0\n");
					break;
				default:
					printf("0 1 ");
					for(i=2;i < n;i++)
					{
						printf("%d ",fibo(i-1) + fibo(i-2));
					}
					printf("\n");
					break;
			}
			break;
		}
		default:
			fprintf(stderr,"Usage: %s <number>\n",argv[0]);
			break;
	}

}
int fibo(int i)
{
	if(i==0) return 0;
	else if(i==1) return 1;
	else
		return fibo(i-1) + fibo(i-2);
}

