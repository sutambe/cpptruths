#include <type_traits>
#include <tuple>

template <typename Sub, typename Super>
struct subset_of : std::false_type {};

template<typename Same, typename... AOther, typename... BOther>
struct subset_of<std::tuple<Same, AOther...>, std::tuple<Same, BOther...>>
: subset_of<
    std::tuple<AOther...>
    , std::tuple<BOther...>
> {};

template<typename ADifferent, typename BDifferent, typename... AOther, typename... BOther>
struct subset_of<std::tuple<ADifferent, AOther...>, std::tuple<BDifferent, BOther...>>
: subset_of<
    std::tuple<ADifferent, AOther...>
    , std::tuple<BOther...>
> {};

template<typename... B>
struct subset_of<std::tuple<>, std::tuple<B...>>: std::true_type {};

template<typename... A>
struct Foo {
    template<typename... B>
    static void bar()
    {
        //static_assert(subset_of<std::tuple<A...>, std::tuple<B...>>::value, "Hello");
        static_assert(subset_of<std::tuple<A...>, std::tuple<B...>>::value, "Hello");
    }
};
struct Apple{}; struct Orange{}; struct Grape{};

int main()
{    
  Foo<Apple, Orange>::bar<Apple, Orange, Grape>();  // this compiles    
  Foo<Apple, Orange>::bar<Grape, Apple, Grape, Orange, Grape>(); // this also compiles    
  Foo<Apple, Orange>::bar<Orange, Grape>();    // this doesn't
}

