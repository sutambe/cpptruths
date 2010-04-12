#include <iostream>

template<class T> // (a) a base template
void f( T ) 
{
  std::cout << "f(T)\n";
}

template <>
void f<>(int*) // explicit specialization
{
  std::cout << "f(int *) after T\n";
}

template<class T> // (b) another, overloads (a)
void f( T* )
{
  std::cout << "f(T *)\n";
}

/*
template<> // explicit specialization
void f<>(int*)
{
  std::cout << "f(int *) after T *\n";
}
*/
/*
Example courtesy Sutter, Abrahams, Dimov
Function templates do not consider specializations 
when doing overload resolution.
Only after a primary template is found are 
specializations are considered.
*/

int main (void)
{
    int *p = 0;
    f(p); 
}
