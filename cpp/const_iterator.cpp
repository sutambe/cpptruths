#include <vector>
#include <iostream>
#include <cxxabi.h>

void func(char *) { }
int main(void)
{
  std::vector<char *> v(5);
  std::vector<char *>::const_iterator iter;
  iter = v.begin();
  //*iter = "abcd";
  int s;
  std::cout << abi::__cxa_demangle(typeid(*iter).name(), 0, 0, &s) << std::endl;

  const std::vector<char *> vc(5);
  //vc[0] = "abcd";
  typedef char * const & char_star_const_amp;
  typedef char * char_ptr;
  typedef char_ptr const & char_ptr_const_amp;
  std::cout << abi::__cxa_demangle(typeid(char_star_const_amp).name(), 0, 0, &s) << std::endl;
  std::cout << abi::__cxa_demangle(typeid(char_ptr_const_amp).name(), 0, 0, &s) << std::endl;

  func(*iter);
}

