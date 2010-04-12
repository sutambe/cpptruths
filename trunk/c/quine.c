#include <stdio.h>

int
main (void)
{
  char *s1="#include <stdio.h>%c%cint%cmain (void)%c{%c";
  char *s2="  char *s%c=%c%s%c;%c  char *s%c=%c%s%c;%c";
  char *s3="  char n='%cn', q='%c', b='%c%c';%c";
  char *sp="  printf(";
  char *s4="%ss1,n,n,n,n,n);%c";
  char *s5="%ss2,'1',q,s1,q,n,'2',q,s2,q,n);%ss2,'3',q,s3,q,n,'p',q,sp,q,n);%c";
  char *s6="%ss2,'4',q,s4,q,n,'5',q,s5,q,n);%ss2,'6',q,s6,q,n,'7',q,s7,q,n);%c";
  char *s7="%ss2,'8',q,s8,q,n,'9',q,s9,q,n);%ss2,'0',q,s0,q,n,'x',q,sx,q,n);%c";
  char *s8="%ss3,b,q,b,b,n);%ss4,sp,n);%ss5,sp,sp,n);%c";
  char *s9="%ss6,sp,sp,n);%ss7,sp,sp,n);%ss8,sp,sp,sp,n);%c";
  char *s0="%ss9,sp,sp,sp,n);%ss0,sp,sp,n,n,n);%c  return 0;%c}%c";
  char *sx="--- This is an intron. ---";
  char n='\n', q='"', b='\\';
  printf(s1,n,n,n,n,n);
  printf(s2,'1',q,s1,q,n,'2',q,s2,q,n);  printf(s2,'3',q,s3,q,n,'p',q,sp,q,n);
  printf(s2,'4',q,s4,q,n,'5',q,s5,q,n);  printf(s2,'6',q,s6,q,n,'7',q,s7,q,n);
  printf(s2,'8',q,s8,q,n,'9',q,s9,q,n);  printf(s2,'0',q,s0,q,n,'x',q,sx,q,n);
  printf(s3,b,q,b,b,n);  printf(s4,sp,n);  printf(s5,sp,sp,n);
  printf(s6,sp,sp,n);  printf(s7,sp,sp,n);  printf(s8,sp,sp,sp,n);
  printf(s9,sp,sp,sp,n);  printf(s0,sp,sp,n,n,n);
  return 0;
}
