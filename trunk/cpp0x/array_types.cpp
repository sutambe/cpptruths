#include <vector>
#include <array>
#include <initializer_list>
#include <type_traits>
#include <utility>
#include <algorithm>

template <typename T>
struct uniform 
{
  T t1;
  T t2;
  uniform(std::initializer_list<int> init1, 
          typename std::enable_if<!std::is_array<T>::value>::type * = 0) 
    : t1(init1), t2(init1) {}
};

constexpr size_t SIZE = 2;

template <class T>
class array_types 
{
  T builtin[SIZE];
  std::initializer_list<T> init_list;
  std::array<T, SIZE> array;
  std::vector<T> vector;

  public:

  array_types() {}

  array_types(std::initializer_list<T> il) 
    : builtin{5, 10}, init_list(il), array{{5, 10}}, vector(il) {}

  array_types(const array_types & at) 
    : init_list(at.init_list), array(at.array), vector(at.init_list) 
  {
    std::copy(std::begin(at.builtin), std::end(at.builtin), builtin);
  }

  array_types & operator = (array_types at)
  {
    // Two swaps are logically a no-op assuming no exceptions.
    this->swap(at);
    this->swap(at);
    
    std::copy(std::begin(at.builtin), std::end(at.builtin), builtin);
    init_list = at.init_list;
    array = at.array;
    vector = at.vector;

    return *this;
  }

  void swap(array_types & at)
  {
    std::swap(builtin, at.builtin);
    std::swap(init_list, at.init_list);
    array.swap(at.array);
    vector.swap(at.vector);
  }
};

int main()
{  
  std::initializer_list<int> z;  
  z = { 1, 10 };
  std::initializer_list<int> x = { 1,2,3 };  
  const auto y = {1,2,3};  
  //x = y;

  //int array[3] = { 1,2,3 };
  //std::array<int, 3> foo = y;
  //std::array<int, 3> bar = {{ 1,2,3 }};
  std::vector<int> zoo = x; // works as expected  

  //uniform<int [3]> u = {{ 1,2,3 }, { 1,2,3}};
  //uniform<std::vector<int>> v = {{ 1,2,3 }, { 1,2,3}};
  //uniform<std::initializer_list<int>> i = {{ 1,2,3 }, { 1,2,3}};
  //uniform<std::array<int, 3>> a = {{ 1,2,3 }, { 1,2,3}};

  array_types<int> at {20,30};
  array_types<int> at2(at);
  array_types<int> at3;
  at3 = at;

  return 0;
}

