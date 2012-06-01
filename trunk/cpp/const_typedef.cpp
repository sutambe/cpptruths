#include <cstdio>

char buffer[] = "Hi";

template <class T>
void read_only_ref(T const & str)
{
  str = buffer+1; // can't modify const
}

template <class T>
void losing_const(T const & str)
{
}

int main(void)
{
  http://drdobbs.com/article/print?articleId=231601151&siteSectionName=
  
  char A = 'A';

  typedef char * ntcs;
  typedef const char * cntcs;
  typedef const char * const cntcsc;

  const ntcs p = &A; // must be initialized
  // p = &A;            // assignment to const not allowed
   
  cntcs q;
  q = &A;
  //*q = 'B';     // Can't modify const

  const cntcs s = &A;  // must be initialized
  // s = &A;      // assignment to const not allowed.
  //*s = 'B';     // Can't modify const

  cntcsc r = &A;     // must be initialized
  // r = &A;            // assignment to const not allowed
  *p = 'B';
  // *q = 'B';          // can't modify const
  // *r = 'B';          // can't modify const
  
  char * const str1 = buffer;
  const char * str2 = buffer;
  //read_only_ref<char *>(str1); 
  //losing_const<char *>(str2);  // not allowed
}

