#include <iostream>
#include <string>

int main (void)
{
  char buf[] = { 'A', 'B', 0, 'C', 0, 'D'};
  std::string str1 (buf); // only creates "AB"
  std::string str2 (buf, buf + sizeof (buf)); // creates "ABCD"

  std::cout << str1 << " length = " << str1.length() << std::endl;
  std::cout << str2 << " length = " << str2.length() << std::endl;

  return 0;
}

