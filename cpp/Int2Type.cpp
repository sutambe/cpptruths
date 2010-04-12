#include <iostream>

template <char I> // Try char instead of int
struct Int2Type
{
    enum { value = I };
    typedef Int2Type<I> type;
    typedef typename Int2Type<I+1>::type next; // Note change here.
};
template <typename T>
struct Type : Type< Type<T> > {};

int main(void)
{
    Type<int> t;
    //Int2Type<5> i;
    //std::cout << i.value << std::endl; 
    return 0;
}

