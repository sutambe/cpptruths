#include <iostream>
#include <string>

void func (const std::string & s)
{
  std::cout << "void func (const std::string & s)\n";
}

void func (std::string & s)
{
  std::cout << "void func (std::string & s)\n";
}

int main(void)
{
  std::string str("Vanderbilt");
  func(str);
}

