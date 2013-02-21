#include <iostream>

namespace track {

class Radar { };
class Missile { };

std::ostream & print(std::ostream & o, const Radar &r)
{
  o << "This is an automatically generated Radar printer.\n";
  return o;
}

std::ostream & print(std::ostream & o, const Missile &m)
{
  o << "This is an automatically generated Missile printer.\n";
  return o;
}

}

namespace common {

template <class T>
std::ostream & operator << (std::ostream & o, const T &t)
{
  return print(o, t);
}

}

namespace track {

std::ostream & operator << (std::ostream & o, const Radar & r)
{
  o << "This is user-defined operator << \n";
  return o;
}
}

int main(void)
{
  using namespace common;

  track::Radar r;
  track::Missile m;
  std::cout << r << m;
}

