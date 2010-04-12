#include <iostream>

static void func (void)
{}

template <class T>
void X (T)
{
}

int main (void)
{
  X(func);
}
