#include <cstdio>

int negate(int i) { return -i; }

template <class F>
//typename F::result_type negate(const F& f) { return negate(f); }
F negate(const F& f) { return f++++; }

template <class T>
struct is_pointer
{ 
  template <class U>
  static char is_ptr(U *);
 
  template <class X, class Y>
  static char is_ptr(Y X::*); 

  template <class U>
  static char is_ptr(U (*)());

  static double is_ptr(...);

  static T t;
  enum { value = sizeof(is_ptr(t)) == sizeof(char) };
};

struct Foo {
  int bar;
};

int main(void)
{
  typedef int * IntPtr;
  typedef int Foo::* FooMemberPtr;
  typedef int (*FuncPtr)();

  negate(1);
  printf("%d\n",is_pointer<IntPtr>::value);
  printf("%d\n",is_pointer<FooMemberPtr>::value);
  printf("%d\n",is_pointer<FuncPtr>::value);
}

