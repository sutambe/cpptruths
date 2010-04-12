#include <iostream>

template <typename T>
T & max(T &a, T &b)
{
    return a > b ? a : b;
}
int main(void)
{
    max("APPLE","PEAR");

}

