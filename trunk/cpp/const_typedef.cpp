#include <cstdio>

int main(void)
{
  http://drdobbs.com/article/print?articleId=231601151&siteSectionName=
  
  char A = 'A';
  typedef char * ntcs;
  typedef const char * cntcs;
  typedef const char * const cntcsc;

  const ntcs p; // must be initialized
  p = &A; // assignment to const not allowed
  cntcs q = &A;
  cntcsc r; // must be initialized
  r = &A;   // assignment to const not allowed
  *p = 'B';
  *q = 'B'; // can't modify const
  *r = 'B'; // can't modify const
}

