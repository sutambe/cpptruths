#include <stdio.h>
#include <stdlib.h>

#define ELEMENTS 5

void nPr (int [], int n, int r);
void perm(int[],int,int, int depth);
void assign(int[],int[]); 
void print(int[], int depth);
void interchange(int *,int,int);


int main(void)
{
	int A[] = {0, 1, 2, 3, 4};

	//perm(A, 0, ELEMENTS, 2);
	nPr (A, ELEMENTS, 3);
	return 0;
}

void nPr (int *A, int n, int r)
{
	perm (A, 0, n, r-1);
}
void perm(int *A,int k, int n, int depth)
{
	int i;
	int B[ELEMENTS];
	if(k > depth)
	{
		print(A, k);
		return;
	}

	assign(B, A);

	for(i=k;i<n;i++)
	{
		interchange(A, k, i);
		perm(A, k+1, n, depth);
		assign(A, B);
	}
}

void print(int *A, int n)
{
	for(int i = 0; i < n; i++)
		printf("%d ",A[i]);
        printf ("\n");
}

void assign(int *B,int *A)
{
	for(int i = 0; i < ELEMENTS; i++)
		B[i] = A[i];
}

void interchange(int *A,int k,int i)
{
	int temp;
	temp = A[i];
	A[i] = A[k];
	A[k] = temp;
}

