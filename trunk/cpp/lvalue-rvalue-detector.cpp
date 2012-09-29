#include <iostream>
#include <cassert>

using namespace std;

struct X {
        X(){}
};
 
X x;
X & xr = x;
const X xc;
 
      X   foo()  { return x; }
const X   fooc() { return x; }
      X & foor()  { return x; }
const X & foorc() { return x; }
 
struct rvalue_probe
{
        template< class R > operator       R () { throw "rvalue"; }
        // template< class R > operator R const () { throw "const rvalue"; } // doesn't work, don't know why
        template< class L > operator       L & () const { throw "lvalue"; }
        template< class L > operator const L & () const { throw "const lvalue"; }
};
 
typedef int lvalue_flag[1];
typedef int rvalue_flag[2];
template <typename T> struct isref     { static const int value = 0; typedef lvalue_flag type; };
template <typename T> struct isref<T&> { static const int value = 1; typedef rvalue_flag type; };
 
template <typename T>
struct nondeducible
{
  typedef T type;
};

char (& is_lvalue_helper(...))[1];

template <typename T>
char (& is_lvalue_helper(T&, typename nondeducible<const volatile T&>::type))[2];

#define is_lvalue(x) (sizeof(is_lvalue_helper((x),(x))) == 2)

struct S
{
  int i;
};

template <typename T>
void test_()
{
  T a = {0};
  T& b = a;
  T (* c)() = 0;
  T& (* d)() = 0;
  assert (is_lvalue(a));
  assert (is_lvalue(b));
  assert (!is_lvalue(c()));
  assert (is_lvalue(d()));
}

template <typename T>
void test()
{
  test_<T>();
  test_<const T>();
  test_<volatile T>();
  test_<const volatile T>();
}

int main() {
        try{ true ? rvalue_probe() : x;       } catch (const char * result) { cout << result << endl; } // Y lvalue
        try{ true ? rvalue_probe() : xc;      } catch (const char * result) { cout << result << endl; } // Y const lvalue
        try{ true ? rvalue_probe() : xr;      } catch (const char * result) { cout << result << endl; } // Y       lvalue
        try{ true ? rvalue_probe() : foo();   } catch (const char * result) { cout << result << endl; } // Y rvalue
        try{ true ? rvalue_probe() : fooc();  } catch (const char * result) { cout << result << endl; } // Y rvalue
        try{ true ? rvalue_probe() : foor();  } catch (const char * result) { cout << result << endl; } // Y lvalue
        try{ true ? rvalue_probe() : foorc(); } catch (const char * result) { cout << result << endl; } // Y const lvalue
 
  test<int>();
  test<S>();
}
