#include <iostream>

/*
Compile with two different sets of command line options.

$ g++ -D LIBRARY_Export command_line_macro.cpp -o command_line_macro

$ g++ -D LIBRARY_Export= command_line_macro.cpp -o command_line_macro

*/

int main (void)
{
  int i = LIBRARY_Export;
  std::cout << i << std::endl;
  return 0;
}

