#include <iostream>
#include <memory>

using namespace std;

struct base1
{ int i; };
struct base2
{ char c[3]; };
struct derived : public base1, public base2
{ double d; };

int main(void)
{
    auto_ptr <derived> d(new derived);
    base1 *b1=d.get();
    base2 *b2=d.get();
    void *v=d.get();

/*
    if(b1==d)
        cout << "b1 is equal to d" << endl;
    if(b2==d)
        cout << "b2 is equal to d" << endl;
    if(b1==v)
        cout << "b1 is equal to v" << endl;
    if(b2==v)
        cout << "b2 is equal to v" << endl;
*/
    return 0;

}

