#include <tuple>

template<class... Arg1>
struct Cartesian
{
  template<class... Arg2>
  struct Inner {
    typedef std::tuple<std::pair<Arg1, Arg2>...> type;
  };
};

int main(void)
{
  Cartesian<char, int, long>::Inner<unsigned char, unsigned int, unsigned long>::type product;

  return 0;
}

