#include <array> 
#include <cstdio>
 
template <class T, unsigned I, unsigned J> 
using Matrix = std::array<std::array<T, J>, I>; 

template <class T, size_t I, size_t... J>
struct MultiDimArray {
  typedef typename MultiDimArray<T, J...>::type Nested;
  typedef std::array<Nested, I> type;
};

template <class T, size_t I>
struct MultiDimArray<T, I> {
  typedef std::array<T, I> type;
};

template <class T, size_t I, size_t J>
union data 
{
  T native[I][J];
  Matrix<T, I, J> matrix; 
  typename MultiDimArray<T, I, J>::type multidim;

  data() : matrix() {}
};

int main() 
{ 
  constexpr size_t I = 3;
  constexpr size_t J = 4;

  data<int, I, J> d;
  size_t val = 0;
  for(size_t i = 0;i < I; ++i)
  {
    for(size_t j = 0;j < J; ++j)
      d.native[i][j] = val++;
  }

  for(size_t i = 0;i < I; ++i)
  {
    for(size_t j = 0;j < J; ++j)
      printf("%d ", d.matrix[i][j]);

    printf("\n");
  }

  printf("\n");

  for(size_t i = 0;i < I; ++i)
  {
    for(size_t j = 0;j < J; ++j)
      printf("%d ", d.multidim[i][j]);

    printf("\n");
  }

  return 0;;
} 

