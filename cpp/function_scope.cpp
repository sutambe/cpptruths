#include <iostream>

template <class T>
class X
{
};

template <class T>
void func (T)
{
}

int main(void)
{
  struct ABCD
  {
    int i;
  };
struct
{
  float d;
} anon;


  ABCD a;
  //X<ABCD> x;
  func(anon);

  return 0;
}

