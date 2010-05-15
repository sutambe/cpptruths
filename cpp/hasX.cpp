#include <iostream>
#include <tr1/type_traits>
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

    typedef char ArrayOfOne[1];
    typedef char ArrayOfTwo[2];

    template<typename C> static ArrayOfOne & f(ChT<int Fallback::*, &C::x>*); 
    template<typename C> static ArrayOfTwo & f(...); 

    //static bool const value = sizeof(f<Derived>(0)) == 2;
    enum { value = sizeof(f<Derived>(0)) == 2 };
}; 



class A { float x; };
struct B { int y; };

#define CREATE_MEMBER_DETECTOR(X)                                                   \
template<typename T> class Detect_##X {                                             \
    struct Fallback { int X; };                                                     \
    struct Derived : T, Fallback { };                                               \
                                                                                    \
    template<typename U, U> struct Check;                                           \
                                                                                    \
    typedef char ArrayOfOne[1];                                                     \
    typedef char ArrayOfTwo[2];                                                     \
                                                                                    \
    template<typename U> static ArrayOfOne & func(Check<int Fallback::*, &U::X> *); \
    template<typename U> static ArrayOfTwo & func(...);                             \
  public:                                                                           \
    typedef Detect_##X type;                                                        \
    enum { value = sizeof(func<Derived>(0)) == 2 };                                 \
};

CREATE_MEMBER_DETECTOR(first);
CREATE_MEMBER_DETECTOR(second);

int main() 
{ 
  std::cout << HasX<A>::value << std::endl; // 1
  std::cout << HasX<B>::value << std::endl; // 0
  typedef std::pair<int, double> Pair;
  std::cout << ((Detect_first<Pair>::value && Detect_second<Pair>::value)? "Pair\n" : "Not Pair\n");
}

