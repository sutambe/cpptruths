#include <iostream>

namespace {

static union  //no tag name
{
  int x;
  char c[4];
}; //no named instances or pointers
x=5; //members of an anonymous union are accessed directly
int first_byte=c[0];

}

int main(void)
{
  x = 5;
  return 0;
}

