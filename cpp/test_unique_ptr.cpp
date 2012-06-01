#include "unique_ptr.hpp"

using boost::unique_ptr;

unique_ptr<int> func(int i)
{
  unique_ptr<int> up(new int(i));
  return move(up);
  //return up;
}

int main(void)
{
  unique_ptr<int> up = move(func(10));
  //unique_ptr<int> up = func(10);
  unique_ptr<int> up2;
  up2 = move(up);

  return 0;
}
