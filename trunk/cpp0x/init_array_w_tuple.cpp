#include <array>
#include <tuple>
#include <iostream>

template <size_t I, typename Array, typename Tuple>
struct Assign
{
  static void execute(Array &a, Tuple const & tuple)
  {
    a[I] = std::get<I>(tuple);
    Assign<I-1, Array, Tuple>::execute(a, tuple);
  }
};

template <typename Array, typename Tuple>
struct Assign <0, Array, Tuple>
{
  static void execute(Array &a, Tuple const & tuple)
  {
    a[0] = std::get<0>(tuple);
  }
};

template <class T, size_t N>
class tuple_array : public std::array<T, N>
{
    typedef std::array<T, N> Super;

  public:
   
    tuple_array(const tuple_array & ta)
      : Super(ta)
    {
      std::cout << "Copy-ctor called. No RVO :(\n";
    }
    
    template<typename Tuple>
    tuple_array(Tuple const & tuple)
      : Super() 
    {
      Assign<std::tuple_size<Tuple>::value-1, Super, Tuple>::execute(*this, tuple);
    }
};

template <typename... Args>
tuple_array<typename std::tuple_element<0, std::tuple<Args...>>::type, sizeof...(Args)> 
make_tuple_array(Args&&... args)
{
  typedef typename std::tuple_element<0, std::tuple<Args...>>::type ArgType;
  typedef tuple_array<ArgType, sizeof...(Args)> TupleArray; 
  return TupleArray(std::tuple<Args...>(std::forward<Args>(args)...)); 
}

int main(void)
{
  auto array = make_tuple_array(10, 20, 30, 40, 50);
  for(size_t i = 0;i < array.size(); ++i)
  {
    std::cout << array[i] << " ";
  }
  std::cout << std::endl;

  return 0;
}
