#include <stdexcept>

constexpr int & func(int i)
{
  return (i==5)? i : throw std::out_of_range("");
}

int main(void)
{
  static_assert(func(5)==25,"..");
}

