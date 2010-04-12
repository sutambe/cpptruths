#include <iostream>

using namespace std;

class B
{
    public:
    void f( double ) { }
};
class D : public B
{
    public:
    using B::f;
    void f( D & ) {}
    //int f;
};

int main(void)
{
    D d;
    d.f( 12.3 ); // confusing
    return 0;
}

