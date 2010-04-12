typedef void *p;
typedef void *q[1];
typedef int (*w)[1];
typedef void (*r)(void *);
typedef int (*s())[1][1];
typedef void (*t())();
typedef void (*(*u())[1])();
typedef void ( *y (int, void (*)(void *) ) ) ();
typedef int MAIN (void);


int main (void)
{
  //(**(void(**)())0L)();
  //t a;
  MAIN *a;
  printf ("%d\n",sizeof(a));
  return 0;
}
/*
typedef void (*t())();
t is a type of a *function* (not a function pointer) that returns a pointer to
a function, which does not specify any input parameters and return none.

typedef int (*s())[1][1];
s is again a type of a function that does not take any parameters
but returns a pointer to a 2 dimensional array of row and column size being 1
each.

typedef void (*(*u())[1])();
u is again a type of a function which returns a pointer to an array of
function pointers of size 1. These function pointers correspond to functions
which do not take any input and return none.

typedef void ( *y (int, void (*)(void *) ) ) ();
y is again a type of function which take an integer and function pointer as
input and returns a pointer to a function which takes nothing and return
nothing.
*/
