#include <vector>
#include <list>
#include <tuple>
#include <string>

template<unsigned...> struct index_tuple{};

template<unsigned I, typename IndexTuple, typename... Types>
struct make_indices_impl;

template<unsigned I, unsigned... Indices, typename T, typename... Types>
struct make_indices_impl<I, index_tuple<Indices...>, T, Types...>
{
  typedef typename make_indices_impl<I + 1, index_tuple<Indices..., I>, Types...>::type type;
};

template<unsigned I, unsigned... Indices>
struct make_indices_impl<I, index_tuple<Indices...> >
{
  typedef index_tuple<Indices...> type;
};

template<typename... Types>
struct make_indices : make_indices_impl<0, index_tuple<>, Types...>
{};

struct Blob
{
  template <typename... Args1, 
            typename... Args2>
  Blob(std::tuple<Args1...> tuple1,
       std::tuple<Args2...> tuple2)
  : Blob(tuple1, 
         tuple2, 
         typename make_indices<Args1...>::type(),
         typename make_indices<Args2...>::type())
  {}

  template <typename... Args1,
            typename... Args2,
            unsigned... Indices1,
            unsigned... Indices2>
  Blob(std::tuple<Args1...> tuple1,
       std::tuple<Args2...> tuple2,
       index_tuple<Indices1...>,
       index_tuple<Indices2...>)
    : v(std::forward<Args1>(std::get<Indices1>(tuple1))...),
      l(std::forward<Args2>(std::get<Indices2>(tuple2))...)
  { }

  std::vector<std::string> v;
  std::list<double> l;
};

int main(void)
{
  std::tuple<const char *> tc;
  std::get<0>(tc) = "abcd";
  Blob b1(std::forward_as_tuple(5, "C++ Truths"), 
          std::forward_as_tuple(10, 20.20));
  Blob b2(std::forward_as_tuple(5), 
          std::forward_as_tuple(10, 20.20));
  Blob b3(std::forward_as_tuple(), 
          std::forward_as_tuple(10, 20.20));
}

