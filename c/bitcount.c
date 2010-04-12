#include <stdio.h>

/*
Fast Bit Counting Routines
Compiled from various sources by Gurmeet Singh Manku 
http://infolab.stanford.edu/~manku/bitcount/bitcount.html

To know whether a number is a power of 2 use !(n & (n-1)) 
*/
int *(*fp())(){}
int BitCount(int n)
{
  int count;
  count = n-((n>>1)&033333333333)-((n>>2)& 011111111111);
  return ((count+(count>>3))&030707070707)%63;
}
int lookup_bitcount (int num)
{
 static const unsigned char LookupTable[] = 
 { 
   0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
   1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
   1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
   2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
   1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
   2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
   2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
   3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
   1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
   2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
   2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
   3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
   2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
   3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
   3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
   4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
 };     

 return   LookupTable[num & 0xff] +
          LookupTable[(num >> 8) & 0xff] +
          LookupTable[(num >> 16) & 0xff] +
          LookupTable[num >> 24];
}

#define TWO(c)     (0x1u << (c))
#define MASK(c)    (((unsigned int)(-1)) / (TWO(TWO(c)) + 1u))
#define COUNT(x,c) ((x) & MASK(c)) + (((x) >> (TWO(c))) & MASK(c))
     
int parallel_count (unsigned int n)
{
  n = COUNT(n, 0) ;
  n = COUNT(n, 1) ;
  n = COUNT(n, 2) ;
  n = COUNT(n, 3) ;
  n = COUNT(n, 4) ;
  /* n = COUNT(n, 5) ;    for 64-bit integers */
                                    
  return n ;
}

#define MASK_01010101 (((unsigned int)(-1))/3)
#define MASK_00110011 (((unsigned int)(-1))/5)
#define MASK_00001111 (((unsigned int)(-1))/17)

int nifty_parallel_count (unsigned int n)
{
   n = (n & MASK_01010101) + ((n >> 1) & MASK_01010101) ; 
   n = (n & MASK_00110011) + ((n >> 2) & MASK_00110011) ; 
   n = (n & MASK_00001111) + ((n >> 4) & MASK_00001111) ; 
   return n % 255 ;
}

int main(int argc, char *argv [])
{
  int n = atoi(argv[1]);
  int p = n;
  int count;
  printf ("Is n a power of two: %d\n",!( n & (n-1)));
  for(count=0;p;p&=p-1,count++);
  
  printf ("%d, BitCount = %d, lookup_bitcount = %d," 
          "parallel_count = %d, nifty_p_count = %d\n", 
           count, BitCount (n), lookup_bitcount (n),
           parallel_count (n), nifty_parallel_count (n));
  return 0;
}
