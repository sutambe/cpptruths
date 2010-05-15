#include <iostream>

template <class Pair>
struct Reverse
{
    typedef typename Pair::first_type  second_type;
    typedef typename Pair::second_type first_type;
    second_type second;
    first_type first;
};

template <class Pair>
Reverse<Pair> & mutate(Pair & p)
{
  return reinterpret_cast<Reverse<Pair> &>(p);
}

int main(void)
{
  std::pair<double, int> p(1.34, 5);

  std::cout << "p.first = " << p.first << ", p.second = "  << p.second << std::endl;
  std::cout << "mutate(p).first = " << mutate(p).first << ", mutate(p).second = "  << mutate(p).second << std::endl;
}

