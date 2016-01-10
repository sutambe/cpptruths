#include <iostream>
#include <experimental/generator>
#include <string>

std::experimental::generator<char> hello(int i) 
{
   for (auto ch : "Hello, world")
      yield ch+i;
}


int main()
{
  int i = 0;

  hello(++i);

  for (auto ch : hello(++i))
      std::cout << ch;

  
	return 0;
}
