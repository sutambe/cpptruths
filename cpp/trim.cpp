#include <iostream>
#include <algorithm>
#include <functional>
#include <string>

// remove trailing whitespace
inline std::string &rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// remove leading whitespace
inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// remove whitespace from both ends
inline std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}

int main(void)
{
  std::string test("  Sumant  ");
  std::cout << trim(test) << "#" << std::endl;
  std::string::reverse_iterator riter = std::find(test.rbegin(), test.rend(), 'm');
  std::string::iterator iter(riter.base());
  std::cout << "*riter = " << *riter << std::endl;
  std::cout << "*riter.base() = " << *iter << std::endl;

  return 0;
}

