#include <stdio.h>
#include <stdlib.h>

#define ELEMENTS 5

void perm(int[],int,int);
void assign(int[],int[]); 
void print(int[]);
void interchange(int *,int,int);

int main(void)
{
	int A[] = {19,39,7,76,45};

	perm(A,0,ELEMENTS);
	return 0;
}

void perm(int *A,int k, int n)
{
	int i;
	int B[ELEMENTS];
	if(k == n)
	{
		print(A);
		return;
	}

	assign(B,A);

	for(i=k;i<n;i++)
	{
		interchange(A,k,i);
		perm(A,k+1,n);
		assign(A,B);
	}
}

void print(int *A)
{
	printf("\n");
	for(int i=0;i<ELEMENTS;i++)
		printf("%d ",A[i]);
}

void assign(int *B,int *A)
{
	for(int i=0;i<ELEMENTS;i++)
		B[i] = A[i];
}

void interchange(int *A,int k,int i)
{
	int temp;
	temp = A[i];
	A[i] = A[k];
	A[k] = temp;
}

