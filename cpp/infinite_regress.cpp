#include <iostream>

struct A : std::pair <int, A *>
{};

int main(void)
{
  A a;
  a.second = &a;
  return 0;
}

