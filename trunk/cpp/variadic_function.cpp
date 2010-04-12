#include <iostream>

void output() 
{ 
  std::cout << '\n'; 
}

template <typename T, typename... P>
void output(T t, P... p)
{
  std::cout << t << ' ';
  output(p...);
}

int main(void)
{
  output("S", 10, 'T');
  return 0;
}

