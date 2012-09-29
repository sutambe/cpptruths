#include <iostream>
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
 
int main() {
        try{ true ? rvalue_probe() : x;       } catch (const char * result) { cout << result << endl; } // Y lvalue
        try{ true ? rvalue_probe() : xc;      } catch (const char * result) { cout << result << endl; } // Y const lvalue
        try{ true ? rvalue_probe() : xr;      } catch (const char * result) { cout << result << endl; } // Y       lvalue
        try{ true ? rvalue_probe() : foo();   } catch (const char * result) { cout << result << endl; } // Y rvalue
        try{ true ? rvalue_probe() : fooc();  } catch (const char * result) { cout << result << endl; } // Y rvalue
        try{ true ? rvalue_probe() : foor();  } catch (const char * result) { cout << result << endl; } // Y lvalue
        try{ true ? rvalue_probe() : foorc(); } catch (const char * result) { cout << result << endl; } // Y const lvalue
 
}
