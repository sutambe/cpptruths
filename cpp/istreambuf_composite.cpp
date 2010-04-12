#include <iostream>
#include <vector>
#include <algorithm>

template <int N>
struct C
{
  char data [N];
  void operator = (char c)
  {
    data [0] = c;
  }
};


typedef C<5> MyType;

namespace std
{

template <>
std::back_insert_iterator<std::vector<MyType> > &
std::back_insert_iterator<std::vector<MyType> >::operator= (const MyType & value)
{
  return *this;
}

}

int main (void)
{

  std::vector <MyType> v;
  std::copy (std::istreambuf_iterator<char> (std::cin),
             std::istreambuf_iterator<char> (),
             std::back_inserter (v));

  return 0;
}
