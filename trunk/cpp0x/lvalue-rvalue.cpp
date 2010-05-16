#include <iostream>

int main (void)
{
     struct S;
     struct T {int m;};
     volatile S *ptr1 = reinterpret_cast <volatile S *> (0x1234);
     volatile T *ptr2 = reinterpret_cast <volatile T *> (0x1234);
     int *iptr = reinterpret_cast <int *> (0x1234);
     int i;
     volatile int p;
     //static_cast<S> (*ptr1);
     *ptr1;
     *ptr2;
     *iptr;
     i;
     p;
}

