#include <cstdio>
#include <iostream>

template<int n> struct confusing
{
    static int q;
};
template<> struct confusing<1>
{
    template<int n>
    struct q
    {
        q(int x)
        {
            printf("Separated syntax and semantics.\n");
        }
        operator int () { return 0; }
    };
};

int y (int) { return 10; } 
char x;
void foo() 
{
  int x, y(int), z;
  x * y(z); // NOTE THIS IS AN EXPRESSION
}

void bar() 
{
  struct x { x(int) {} } *z;
  x * y(z); // NOTE THIS IS A DECLARATION
}

int main()
{
/*
Parse trees are totally different.
If you "didn't care" about semantics during parsing, 
then confusing<1>::q is a typename, so confusing<1>::q<3>(2) 
creates an object of type confusing<1>::q<3> with the argument 2.

If you "do" semantics during the syntax pass, then confusing<4> will 
be looked up, confusing<4>::q is a variable. The declaration would "expand" to int x = (confusing<4>::q < 3) > 2. 
*/
    char x = confusing<sizeof(x)>::q < 3 > (2);
    std:: cout << x << std::endl;
    return 0;
}

