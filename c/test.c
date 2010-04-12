#include <stdio.h>
#include <stdlib.h>

int main (void)
{       
  //char *x = strdup("GIRL"); // This does not give seg-fault.
  char *x = "GIRL"; /// This gives seg-fault but not the above statement.
  int n = strlen (x);
  
  *x = x[n];
  printf ("%s\n",x+1);

  return 0;
}

/*
int main(void)
{
    /// valgrind test
    int * i = malloc (sizeof(int));
    free (i);
    /// In C you can legally access the free'ed pointer
    /// unless and until you do next allocation using malloc.
    /// This was used as an optimisation in some programs.
    *i = 4;

    return 0;
}
*/
/*
#include <stdio.h>

int main(void)
{
    int p[5][2];
    int *ptr = p[0];
    int **ptrptr = &p;
    printf("%d:%p",sizeof(p),&p);
    return 0;
}
*/
/*
#include <stdio.h>
#include <stdlib.h>
void foo(int array[static 99]);

void funcWithVLA(int n)
{
    // Note that during the lifetime of a vla, its size does not change
    int vla[n];
    int *p=vla;
    n/=2;
    // sizeof eval'd at run-time not compile-time
    printf("%d\n", sizeof(vla)); // sizeof(int) * n
}
// Use * only in prototype, not def
// * indicates unspecified dimension
void funcTakingVLA(int size, int a[*]);

int Global = 99; // boo hiss
//int GlobalVLA[Global]; // Error: VLA can't be extern
                       // extern's need constant for dimension

int main(int argc, char *(*argv)[], char *env[])
{
    int n = -99;
    int m = 999;
    extern int externVla[n]; // Error: VLA still can't be extern
    static int staticVla[n]; // Error: VLA can't be static either
    static int (*p)[n]; // ok: p has block scope and is *ptr* to a VLA
    int unknownSize[*]; // Error: VLA with unspecified bounds
                        // only allowed in function prototype scope
    struct blah {
        int vlamember[n]; // Error: struct members can't be VLAs
    };

    // Eval n at runtime, therefore autoVLA is a VLA
    int autoVLA[n]; // ok
    int twoD[n][m]; // ok: 2d array, and so on

    int randVla[rand()]; // :)  size expression not constant, watch out for 0

    n = -99;
    int negVla[n]; // Error: size shall evaluate > 0
                   // This will normally be a runtime thing, not compile time

    // n eval'd here, not when VlaTD used
    typedef int VlaTD[n]; // ok: has block scope
    VlaTD  someid; // ok

    funcWithVLA(n);
    funcTakingVLA(n, autoVLA);

    // Remember, const is readonly in C, not as constant'y as in C++
    const int dimension = 99;
    int StillAVLA[dimension]; // Therefore, this is a VLA

    // VLA in for loop just fine:
    for(int VLAInForLoop[dimension], i = 0; i < dimension; i++) {
        // ...
        // code apparently using VLAInForLoop
        // ...
    }
    return 0;
}
void funcTakingVLA(int size, int a[size])
{
}
*/
/*
// Compound literals avoid needing to create "funny named" object that
// might otherwise serve no other purpose but to be pointed to, etc.
// Some argue they are similar in nature to C++ construct calls.
//
// Compound literals are composed with a cast-like-kinda construct
// with a brace'd initializer to create unnamed array's.
#include <stdio.h>
int foo() { return 99; }

struct xyz { int i; int j; };
void bar(struct xyz dummy_arg) { }
void baz(struct xyz* dummy_arg) { }

// Initializers for the literals are required to be constant if in file scope
// Unnamed array is static if in file scope
int *p = (int []){ 1, 2 }; // p points to unnamed array of int[2]

int main()
{
    // non-constant initializers ok in enclosing block
    // unnamed array is auto here
    // foo() only done once
    int *p2 = (int []){ 1, foo() }; // ok
    int *z;

    //(int []){ 1, 2 } = 99; // Error
    *(int []){ 1, 2 } = 99; // Ok, but yuck

    // Note the literal need not always just be an array
    p = &(int){ 1 }; // ok, but be careful
    z = &(int){ 1 }; // ok, but be careful
    printf("%p",&(int){1});

    // const compound literals need not be distinct
    const int *cix = (const int []){ 1, 2 };
    const int *ciy = (const int []){ 1, 2 }; // Same literal?

    // String literals do not have const in C. This alternative
    // might help in some cases (perhaps wrapped in a macro??):
    const char *ccp = (const char []){ "Comeau C/C++" };

    // Pass a compound literal to a function
    bar((struct xyz){ 123, 456 });
    // Combo'd with DI's from above:
    bar((struct xyz){ .i = 123, .j = 456 });

    // Pass addr of literals:
    baz(&(struct xyz){ 456, 789 });

    return 0;
}
*/
/*
#include <stdio.h>
#include <stdbool.h>

struct xyz { int a; int b; int c[99]; };
// DI allow for that members can be named in initializer lists
// Note that member c is "zero'd" since it is not uttered in the init list
struct xyz AnXyz = { .b = 99, .a = -99 };
struct xyz AnXyz2 = { .b = 99 };

// DI's ok for array objects too
struct xyz AnXyz3 = { .c[2] = 44 };
int array[99] = { [33] = 33, [66] = 66 };
struct xyz AnXyz3b[] = { [5].c[2] = 44 };

struct abc { int d; struct xyz xyzmem; };
// DIs ok for subobject too
struct abc AnAbc = { .d = 11, .xyzmem.b = 33 };

// "weird" stuff too: take last DI, but eval all
// Look: member b done twice, last one "wins"
struct abc AnAbc2 = { .d = 11, .d = 22 };
int foo() { static int i=10; return i++; }
void bar()
{
    // Function necessary because foo() is not constant
    struct abc AnAbc3 = { foo() , .d=22};
    printf("%d\n",AnAbc3.d);
}

int main()
{
    // unions ok too
    union U { int intmem; float floatmem; };
    union U u = { .floatmem = 99.99 };
    union U u2 = { .intmem = AnXyz2.b + 99 };
    bar();
    bar();
    return 0;
}
*/
/*
#include <stdio.h>
#include <stdbool.h>

typedef int *IntPtr;
const IntPtr p=0;

int main(void)
{
    bool b;
    p++;
    return 0;
}
*/
/*
#include <stdio.h>

const char *const p="SUMANT";
extern void *v=0;

int main(int argc, char **argv, char *env[])
{
float a=0.7;
long double d;
int i;
int *l=(int *)1000,*m=(int *)2000;
//char name[50]={ [0]=p[5] };
char myarray[30];
char *c;
char (*array_ptr)[3];
extern void *v;
//printf("%s %s",(*argv),env[2]);
++v;
printf("%u\n",v);
array_ptr=&myarray;

if(a != 0.7L)
printf("stoned %lf %u\n",a,sizeof(0.7));
else
printf("avenged %lf %u\n",a,sizeof(0.7));

void)scanf("SUMANT %i",&i);
printf("i = %d, long double %u\n",i,sizeof(d));

printf("%s %d\n",name,m-l);
//printf("%d, name = %s",scanf(" P\tQ\nR%[a-z]s",name),name);

return 0;
}
*/
/* gcc test.c -o test -Wall -ansi -pedantic
#include <stdio.h>
#include <malloc.h>

#define SIZE 0

int (*p(char * p))[SIZE]
{
    int (*s)[SIZE];
    printf("%s",p);
    s=(int (*)[SIZE])malloc(SIZE);
    (*s)[0]=65;
    return s;
}
int main(void)
{
      int (*s)[SIZE];
      // int (*p(char *))[SIZE]; What is this declaration all about?
      //   p is a function which takes char * as an argumant and
      //   returns a pointer to 10 elements integer array.

      s=p("SUMANT ");
      printf("%d %d %d\n",(*s)[0],sizeof(s),sizeof(*s));

      return 0;
}
*/
/*#include <stdio.h>

int main(void)
{

int x,y,z;

x=y=z=1;
z = ++x || ++y && ++z;
printf("x=%d ,y=%d,z=%d",x,y,z);

return 0;
}
*/
/*
#include <stdio.h>
void f() {
       int x = 0;
  x = x + 1;
  y = x;
}

int y = 0;
void g() {
  static int x = 0;
  x = x + 2;
  y = x;
}
main() {
  int n = 0;
  while (y < 10) {
    n = n + 1;
    f();
    g();
    printf("n = %d, y = %d\n",n,y);
  }
}
*/


/*
#include <stdio.h>

int main(void)
{
    int a,b,c;
    a=strcomp("xyz", "xzz");
    b=strcomp("yyy", "yyy");
    c=strcomp("abc", "def");


    printf("-1 %d, 0 %d, -3 %d\n",a,b,c);

    return 0;
}

strcomp(char *s, char *t)

{

   while ( *s == *t ) {

      if (*s == '\0')

        return(0);

      s++;

      t++;

   }

   return(*s - *t);

}

*/

