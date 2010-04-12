#include <iostream>

template <typename T>
void free_memory (T)
{ }

template <>
void free_memory (double);

template <>
void free_memory (double)
{
  std::cout << "double" << std::endl;
}


int main (void)
{
  free_memory (1.0);

  return 0;
}
