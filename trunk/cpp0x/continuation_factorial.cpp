#include <iostream>
#include <functional>
#include <cmath>
using namespace std;

template<typename F, typename A1, typename A2, typename K>
void cps2(F f, A1 a1, A2 a2, K k) { k(f(a1, a2)); }

template<typename F1, typename F2>
void cps_ifthen(bool x, F1 k1, F2 k2) {x ? k1() : k2();}

// Factorial using continuation-passing style
void factorial(int n, function<void (int)> k)
{
    cps2(equal_to<int>(), 0, n, 
        [&](bool b){cps_ifthen(b,
            [&](){k(1);},
            [&](){cps2(minus<int>(), n, 1,  
                [&](int nm1){factorial(nm1, 
                    [&](int f){cps2(multiplies<int>(), n, f, k);});});});});
}
int main()
{
    for(int i=0; i<10; ++i) {
        cout << "F("<<i<<"): " << tgamma(i+1) << " or ";
        factorial(i, [](int a) {cout << a << '\n';});
    }
}

