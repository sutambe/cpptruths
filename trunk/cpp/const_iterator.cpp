#include <vector>
#include <iostream>
#include <cxxabi.h>

void func(char *) { }
int main(void)
{
  std::vector<char *> v(5);
  std::vector<char *>::const_iterator iter;
  iter = v.begin();
  *iter = "abcd";
  int s;
  std::cout << abi::__cxa_demangle(typeid(*iter).name(), 0, 0, &s) << std::endl;
  func(*iter);
}

