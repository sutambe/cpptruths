#include <iostream>
/*
 * SFINAE on steroids!
 * http://cplusplus.co.il/2009/09/12/substitution-failure-is-not-an-error-2/
 */

template<typename T> 
struct HasX 
{ 
    struct Fallback { int x; }; // introduce member name "x"
    struct Derived : T, Fallback { };

    template<typename C, C> struct ChT; 

    template<typename C> static char (&f(ChT<int Fallback::*, &C::x>*))[1]; 
    template<typename C> static char (&f(...))[2]; 

    static bool const value = sizeof(f<Derived>(0)) == 2;
}; 

struct A { int x; };
struct B { int y; };

int main() { 
    std::cout << HasX<A>::value << std::endl; // 1
    std::cout << HasX<B>::value << std::endl; // 0
}

