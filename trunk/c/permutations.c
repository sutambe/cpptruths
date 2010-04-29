#include <stdio.h>
#include <stdlib.h>

#define ELEMENTS 4

void nPr(int [], int n, int r);
void perm(int[],int,int, int depth);
void assign(int[],int[]); 
void print(int[], int depth);
void swap(int *,int *);

int main(void)
{
  int A[] = {1, 2, 3, 4};

  //perm(A, 0, ELEMENTS, 2);
  nPr (A, ELEMENTS, ELEMENTS);
  return 0;
}

void nPr (int *A, int n, int r)
{
  perm (A, 0, n, r-1);
}

void perm(int *A, int k, int n, int depth)
{
  int i = 0;
  int B[ELEMENTS] = {0};

  assign(B, A);

  for(i=k;i<n;i++)
  {
    swap(&A[k], &A[i]);
    if(k==depth)
      print(A, k+1);
    else
      perm(A, k+1, n, depth);
    assign(A, B);
  }
}

void print(int *A, int n)
{
  int i = 0;
  for(i = 0; i < n; i++)
    printf("%d ",A[i]);
  printf ("\n");
}

void assign(int *B,int *A)
{
  int i = 0;
  for(i = 0; i < ELEMENTS; i++)
    B[i] = A[i];
}

void swap(int *a, int *b)
{
  int temp = *a;
  *a = *b;
  *b = temp;
}

