#include <vector>
#include <list>
#include <tuple>
#include <string>
#include <utility>

#ifdef MY_INDEX_MAKER

template<unsigned...> struct index_tuple{};

template<unsigned I, typename IndexTuple, typename... Types>
struct make_indices_impl;

template<unsigned I, unsigned... Indices, typename T, typename... Types>
struct make_indices_impl<I, index_tuple<Indices...>, T, Types...>
{
  typedef typename make_indices_impl<I + 1, index_tuple<Indices..., I>, Types...>::TYPE TYPE;
};

template<unsigned I, unsigned... Indices>
struct make_indices_impl<I, index_tuple<Indices...> >
{
  typedef index_tuple<Indices...> TYPE;
};

template<typename... Types>
struct make_indices : make_indices_impl<0, index_tuple<>, Types...>
{};

#endif

#ifdef GCC_INDEX_MAKER

template <class... Args>
using make_indices = std::_Build_index_tuple<sizeof...(Args)>;

template <size_t... Indices>
using index_tuple = std::_Index_tuple<Indices...>;

#define TYPE __type

#endif 

#ifdef CLANG_INDEX_MAKER

template <class... Args>
using make_indices = std::__make_tuple_indices<sizeof...(Args)>;

template <size_t... Indices>
using index_tuple = std::__tuple_indices<Indices...>;

#define TYPE type

#endif 

struct Blob
{
  template <typename... Args1, 
            typename... Args2>
  Blob(std::piecewise_construct_t,
       std::tuple<Args1...> tuple1,
       std::tuple<Args2...> tuple2)
  : Blob(std::move(tuple1), 
         std::move(tuple2), 
         typename make_indices<Args1...>::TYPE(),
         typename make_indices<Args2...>::TYPE())
  {}

private:

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
  Blob b1(std::piecewise_construct,
          std::forward_as_tuple(5, "C++ Truths"), 
          std::forward_as_tuple(10, 20.20));
/*  Blob b2(std::piecewise_construct,
          std::forward_as_tuple(5), 
          std::forward_as_tuple(10, 20.20));
  Blob b3(std::piecewise_construct,
          std::forward_as_tuple(), 
          std::forward_as_tuple(10, 20.20));*/
}

