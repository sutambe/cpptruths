#include <iostream>
#include <string>
#include <tuple>
#include <memory>
#include <boost/core/demangle.hpp>

template <char... chars>
using stream = std::integer_sequence<char, chars...>;

template <typename T, T... chars>
constexpr stream<chars...> operator""_stream() { return { }; }

template <class Head, class Tuple>
struct Append;

template <class Head, class... Args>
struct Append<Head, std::tuple<Args...>>
{
  using type = std::tuple<Head, std::tuple<Args...>>;
};

template <class IntegerSequence>
struct StringToTuple;

template <>
struct StringToTuple<std::integer_sequence<char>>
{
    using type = std::tuple<>;
};

template <char C, char... chars>
struct StringToTuple<std::integer_sequence<char, C, chars...>>
{
    using type = typename StringToTuple<std::integer_sequence<char, chars...>>::type;
};

template <char... chars>
struct StringToTuple<std::integer_sequence<char, '%', 's', chars...>>
{
    using tail = typename StringToTuple<std::integer_sequence<char, chars...>>::type;
    using type = typename Append<std::string, tail>::type;
};

template <char... chars>
struct StringToTuple<std::integer_sequence<char, '%', 'd', chars...>>
{
    using tail = typename StringToTuple<std::integer_sequence<char, chars...>>::type;
    using type = typename Append<int, tail>::type;
};

template <char... chars>
struct StringToTuple<std::integer_sequence<char, '%', 'f', chars...>>
{
    using tail = typename StringToTuple<std::integer_sequence<char, chars...>>::type;
    using type = typename Append<double, tail>::type;
};

template <char... chars>
struct StringToTuple<std::integer_sequence<char, '%', 'u', chars...>>
{
    using tail = typename StringToTuple<std::integer_sequence<char, chars...>>::type;
    using type = typename Append<unsigned int, tail>::type;
};

template <class Tuple>
struct curry;

template <class Head, class Tail>
struct curry<std::tuple<Head, Tail>>
{
    template<class... Args>
    static auto apply(Args&&... args) 
    {
      return [args...](Head h) {
          return curry<Tail>::apply(args..., h);
      };
    }
};

template <class Head>
struct curry<std::tuple<Head, std::tuple<>>>
{
    template <class... Args>
    static auto apply(Args&&... args) {
        return [args...](Head h) { 
            return printf(args..., h); 
        };
    }
};

template <class IntSeq>
auto curried_printf_impl(const char *fmt, IntSeq)
{
  using FormatType = typename StringToTuple<IntSeq>::type;
  std::cout << boost::core::demangle(typeid(FormatType).name()) << "\n";
  return curry<FormatType>::apply(fmt);  
}

#define curried_printf(X) curried_printf_impl(X, X##_stream)

int main(void) 
{
  curried_printf("C++ Rocks %d %f!\n")(10)(20.30);
    
  return 0;
}
