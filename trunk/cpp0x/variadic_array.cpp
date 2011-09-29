#include <cstdio>

template<char c> 
struct add_three 
{
    enum { value = c+3 };
};

template <char... Chars> 
struct EncryptCharsA 
{
    static const char value[sizeof...(Chars) + 1];
};

template<char... Chars>
char const EncryptCharsA<Chars...>::value[sizeof...(Chars) + 1] = {
    //add_three<Chars>::value...
    (Chars+3)...
};

int main() 
{   
    printf("%s\n", EncryptCharsA<'A','B','C'>::value);
    // prints "DEF"
}
