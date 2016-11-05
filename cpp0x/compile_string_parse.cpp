#include <iostream>
#include <string>
#include <tuple>
#include <memory>
#include <boost/core/demangle.hpp>

template <char... chars>
using CharSeq = std::integer_sequence<char, chars...>;

template <typename T, T... chars>
constexpr CharSeq<chars...> operator""_lift() { return { }; }

template <class Head, class Tuple>
struct Append;

template <class Head, class... Args>
struct Append<Head, std::tuple<Args...>>
{
  using type = std::tuple<Head, Args...>;
};

template <class CharSeq>
struct StringToTuple;

template <>
struct StringToTuple<CharSeq<>>
{
    using type = std::tuple<>;
};

template <char C, char... chars>
struct StringToTuple<CharSeq<C, chars...>>
{
    using type = typename StringToTuple<CharSeq<chars...>>::type;
};

template <char... chars>
struct StringToTuple<CharSeq<'%', 's', chars...>>
{
    using tail = typename StringToTuple<CharSeq<chars...>>::type;
    using type = typename Append<const char *, tail>::type;
};

template <char... chars>
struct StringToTuple<CharSeq<'%', 'd', chars...>>
{
    using tail = typename StringToTuple<CharSeq<chars...>>::type;
    using type = typename Append<int, tail>::type;
};

template <char... chars>
struct StringToTuple<CharSeq<'%', 'f', chars...>>
{
    using tail = typename StringToTuple<CharSeq<chars...>>::type;
    using type = typename Append<double, tail>::type;
};

template <char... chars>
struct StringToTuple<CharSeq<'%', 'u', chars...>>
{
    using tail = typename StringToTuple<CharSeq<chars...>>::type;
    using type = typename Append<unsigned int, tail>::type;
};

template <class Tuple>
struct curry;

template <class Head, class... Tail>
struct curry<std::tuple<Head, Tail...>>
{
    template<class... Args>
    static auto apply(Args&&... args) 
    {
      return [args...](Head h) {
          return curry<std::tuple<Tail...>>::apply(args..., std::move(h));
      };
    }
};

template <class Head>
struct curry<std::tuple<Head>>
{
    template <class... Args>
    static auto apply(Args&&... args) {
        return [args...](Head h) { 
            return printf(args..., h); 
        };
    }
};

template <>
struct curry<std::tuple<>>
{
    static auto apply(const char * fmt) {
       return printf(fmt); 
    }
};

template <class CharSeq>
auto curried_printf_impl(const char * const fmt, CharSeq)
{
  using FormatType = typename StringToTuple<CharSeq>::type;
  std::cout << boost::core::demangle(typeid(FormatType).name()) << "\n";
  return curry<FormatType>::apply(fmt);  
}

#define curried_printf(X) curried_printf_impl(X, X##_lift)

int main(void) 
{
  curried_printf("C++ Rocks%s %d %f\n")("!!")(10)(20.30);
  curried_printf("C++ Rocks!\n");
    
  return 0;
}
