#include <iostream>
#include <iomanip>
#include <conio.h>

static int para=200;

void g(int x=para); // default argument is a static variable.
void f(int x=7); // default argument implemented in terms of some static varible.

int main(void)
{
    void f(int x=70);  // redeclaring function ::f

    f(); // prints f70

    g();  // prints g200
    para=500; 
    g(); // prints g500

    {
        void f(int x=700);  // redeclaring function f
        f(); // prints f700
        ::g(); // prints g500
    }

    ::f(); // prints f7  !!!!
    // Note that earlier f() call in the same scope gave us f70!!
    // This shows that :: (scope resolution operator) forces compiler to
    // use global declaration with global signature's default value.
 
    {
       void g(int x=100); // redeclaring function g
       g();   // prints g100!!!
       std::cout << "para = " << para << std::endl; // prints para = 500
       // Note that though value of para is unchaged local scope
       // changes value of default argument.
    }
    ::g(); // prints g500
    getch();
    return 0;
}

void f(int x)
{
    std::cout << "f" << x << std::endl;
}

void g(int x)
{
    std::cout << "g" << x << std::endl;
}
