#include <iostream>
#include <tuple>
#include <memory>
#include <type_traits>
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
    using type = typename Append<const char *, tail>::type;
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

template <class Head, class... Tail>
struct curry<std::tuple<Head, Tail...>>
{
    template<class... Args>
    static auto apply(Args&&... args) 
    {
      return [args...](Head h) {
          return curry<std::tuple<Tail...>>::apply(args..., h);
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

template <class IntSeq>
auto curried_printf_impl(const char * const fmt, IntSeq)
{
  using FormatType = typename StringToTuple<IntSeq>::type;
  std::cout << boost::core::demangle(typeid(FormatType).name()) << "\n";
  return curry<FormatType>::apply(fmt);  
}

#define curried_printf(X) curried_printf_impl(X, X##_lift)

template <size_t... Indices, class Tuple, class Func>
auto execute(std::integer_sequence<size_t, Indices...>,
             Tuple& tuple,
             Func&& func)
{
  return func(std::get<Indices>(tuple)...);
}

template <int I, class AllArgs, class Tuple>
struct dyn_curry;

template <int I, class AllArgs, class Head, class... Tail>
struct dyn_curry<I, AllArgs, std::tuple<Head, Tail...>>
{
    enum { Index = std::tuple_size<AllArgs>::value - I };

    template <class Func>
    static auto apply(std::shared_ptr<AllArgs> shptr, Func&& func)
    {
      return [shptr, func=std::move(func)](const Head &h) mutable {
        std::get<Index>(*shptr) = h;
        return dyn_curry<I-1, AllArgs, std::tuple<Tail...>>::apply(shptr, std::move(func));
      };
    }    
};

template <class AllArgs, class Head>
struct dyn_curry<1, AllArgs, std::tuple<Head>>
{
    enum { Index = std::tuple_size<AllArgs>::value - 1 };
    using IntSeq = std::make_index_sequence<std::tuple_size<AllArgs>::value>;

    template <class Func>
    static auto apply(std::shared_ptr<AllArgs> shptr, Func&& func)
    {
      return [shptr, func=std::move(func)](const Head &h) mutable {
        std::get<Index>(*shptr) = h;
        return execute(IntSeq(), *shptr, std::move(func));
      };
    }    
};

template <class Ret, class... Args>
auto arb_curry(Ret (&func) (Args...))
{
  using AllArgs = std::tuple<std::remove_const_t<std::remove_reference_t<Args>>...>;
  std::cout << boost::core::demangle(typeid(AllArgs).name()) << "\n";
  std::shared_ptr<AllArgs> shptr(new AllArgs);

  return dyn_curry<std::tuple_size<AllArgs>::value, AllArgs, AllArgs>::apply(shptr, func); 
}

template <class Ret>
Ret arb_curry(Ret (&func) ()) { return func(); }

int add(int &i, int &j, int k) { return i+j+k;   }

int identity(int i) { return i; }

int foo() { return printf("foo\n"); }

int main(void) 
{
  curried_printf("C++ Rocks! %d %f\n")(10)(20.30);
  curried_printf("C++ Rocks!\n");
  arb_curry(foo);
  std::cout << arb_curry(identity)(99) << std::endl;
  auto a = arb_curry(add);
  auto b = a(10);
  auto c = b(20);
  auto d = c(30);
  std::cout << d << std::endl;
    
  return 0;
}
