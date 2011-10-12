#include <array> 
#include <cstdio>
#include <typeinfo>
 
#ifdef __clang__

template <class T, unsigned I, unsigned J> 
using Matrix = std::array<std::array<T, J>, I>; 

template <class T, unsigned I, unsigned J> 
using NativeMatrix = T [I][J]; 

#endif

template <class T, size_t I, size_t... J>
struct MultiDimArray {
  using Nested = typename MultiDimArray<T, J...>::type;
  using type = std::array<Nested, I>;
};

template <class T, size_t I>
struct MultiDimArray<T, I> {
  using type = std::array<T, I>;
};

template <class T, size_t I, size_t... J>
struct MultiDimNative {
  using Nested = typename MultiDimNative<T, J...>::type;
  using type = Nested [I];
};

template <class T, size_t I>
struct MultiDimNative<T, I> {
  using type = T [I];
};

template <class T, size_t I, size_t J>
union data 
{
  T native[I][J];
#ifdef __clang__
  NativeMatrix<T, I, J> native_matrix;
  Matrix<T, I, J> matrix; 
#endif
  typename MultiDimArray<T, I, J>::type multidim_array;
  typename MultiDimNative<T, I, J>::type multidim_native;
};

template <class T>
void print(T & t, size_t rows, size_t columns)
{
  printf("Printing %s\n", typeid(T).name());
  for(size_t i = 0;i < rows; ++i)
  {
    for(size_t j = 0;j < columns; ++j)
      printf("%d ", t[i][j]);

    printf("\n");
  }
  printf("\n");
}

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

  print(d.native, I, J);
#ifdef __clang__
  print(d.native_matrix, I, J);
  print(d.matrix, I, J);
#endif
  print(d.multidim_array, I, J);
  print(d.multidim_native, I, J);

  MultiDimNative<double, 3, 4, 5, 6, 7>::type doubles;
  MultiDimArray<float,   3, 4, 5, 6, 7>::type floats;

  printf("sizeof(doubles) = %d.\n", sizeof(doubles));
  printf("sizeof(floats) = %d.\n", sizeof(floats));
  
  //using P = [](double)->void;
  //printf("P = %s\n", typeid(P).name());
  
  return 0;;
} 

