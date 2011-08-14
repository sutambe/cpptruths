#include <utility>
#include <tuple>
#include <vector>
#include <math>

template <class T>
struct Uniform {
  T t;
  Uniform() : t{10, 12} {}
};

int main(void)
{
  Uniform<std::pair<int, int>> p;
  Uniform<std::tuple<int, int>> t;
  Uniform<int [2]> a; 
  Uniform<std::vector<int>> v; // Uses initializer_list 

  return 0;
}
