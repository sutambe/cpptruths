#include "boost/mpl/vector.hpp"

template <typename... Args>
struct X
{
  typedef boost::mpl::vector<Args...> type;
};

int main (void)
{
  X<int>::type VInt;
}

