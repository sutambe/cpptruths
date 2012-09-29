#include <cstddef>
#include <tuple>
#include <utility>
#include <iostream>

template<std::size_t...> struct index_tuple{};

template<std::size_t I, typename IndexTuple, typename... Types>
struct make_indices_impl;

template<std::size_t I, std::size_t... Indices, typename T, typename... Types>
struct make_indices_impl<I, index_tuple<Indices...>, T, Types...>
{
  typedef typename make_indices_impl<I + 1, index_tuple<Indices..., I>, Types...>::type type;
};

template<std::size_t I, std::size_t... Indices>
struct make_indices_impl<I, index_tuple<Indices...> >
{
  typedef index_tuple<Indices...> type;
};

template<typename... Types>
struct make_indices : make_indices_impl<0, index_tuple<>, Types...>
{};

template <class... Args>
void some_func(Args&&...){}

template <class... Args>
class store
{
public:
  store(Args... args) : members(args...) {}
  void forward()
  {
    typedef typename make_indices<Args...>::type Indices;
    return forward0(Indices(), members);
  }

private:
  template <std::size_t... Indices>
  static void forward0(index_tuple<Indices...>, 
                       std::tuple<Args...> & args)
  {
    some_func(std::forward<Args>(std::get<Indices>(args))...);
  }

  std::tuple<Args...> members;
};

template <std::size_t... Indices, class... Args, class Ret>
Ret forward_impl(index_tuple<Indices...>, 
                 std::tuple<Args...> tuple, 
                 Ret (*fptr) (Args...))
{
  return fptr(std::forward<Args>(std::get<Indices>(tuple))...);
}

template<class... Args, class Ret>
Ret forward(std::tuple<Args...> && tuple, Ret (*fptr) (Args...))
{
    typedef typename make_indices<Args...>::type Indices;
    return forward_impl(Indices(), 
                        std::forward<std::tuple<Args...>>(tuple), 
                        fptr);
}

int myfunc(int i, bool, double)
{
  return 5 + i;
}

int main()
{
  store<int, bool, double>(42, true, 1.2).forward();
  std::cout << forward(std::make_tuple(42, true, 1.2), myfunc) << std::endl;
}

