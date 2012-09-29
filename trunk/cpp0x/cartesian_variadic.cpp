#include <iostream>
#include <typeinfo>

#ifdef GCC
  #include <cxxabi.h>
#endif

template<typename...> struct type_list {};

template<typename T1, typename T2> struct pair {};

template<typename T, typename... Rest>
  struct row
{
  typedef type_list<pair<T,Rest>...> type;
};

template <typename... T> struct concat;
template <typename... S, typename... T>
struct concat<type_list<S...>, type_list<T...>>
{
    typedef type_list<S..., T...> type;
};

template <typename... T>
struct expand
{
    typedef type_list<T...> type;
};
template <> struct expand<> { typedef type_list<> type; };
template <typename... T, typename... L>
struct expand<type_list<T...>, L...>
{
    typedef typename concat<typename expand<T...>::type, 
                            typename expand<L...>::type>::type type;
};

template<typename... T>
  struct cross_product
{
    typedef typename expand<type_list<typename row<T,T...>::type...>>::type type;

};

template <int I>
struct A {};

template <class... T>
struct SizeOf {};

template <class... Pairs>
struct SizeOf<type_list<Pairs...>>
{
  enum { value = sizeof...(Pairs) };
};

int main()
{
  typedef cross_product<int, float, short>::type result;
#ifdef GCC
  int s;
  std::cout << abi::__cxa_demangle(typeid(result).name(), 0, 0, &s) << std::endl;
#endif
  std::cout << SizeOf<result>::value << std::endl;

  typedef cross_product<
             A<1> , A<2> , A<3> , A<4> , A<5> , A<6> , A<7> , A<8> , A<9> , A<10>,
             A<11>, A<12>, A<13>, A<14>, A<15>, A<16>, A<17>, A<18>, A<19>, A<20>,
             A<21>, A<22>, A<23>, A<24>, A<25>, A<26>, A<27>, A<28>, A<29>, A<30>,
             A<31>, A<32>, A<33>, A<34>, A<35>, A<36>, A<37>, A<38>, A<39>, A<40>,
             A<41>, A<42>, A<43>, A<44>, A<45>, A<46>, A<47>, A<48>, A<49>, A<50>,
             A<51>, A<52>, A<53>, A<54>, A<55>, A<56>, A<57>, A<58>, A<59>, A<60>,
             A<61>, A<62>, A<63>, A<64>, A<65>, A<66>, A<67>, A<68>, A<69>, A<70>,
             A<71>, A<72>, A<77>, A<74>, A<75>, A<76>, A<77>, A<78>, A<79>, A<80>,
             A<81>, A<82>, A<83>, A<84>, A<85>, A<86>, A<87>, A<88>, A<89>, A<90>,
             A<91>, A<92>, A<93>, A<94>, A<95>, A<96>, A<97>, A<98>, A<99>, A<100> >::type TypesNxN;
#ifdef GCC
  std::cout << abi::__cxa_demangle(typeid(TypesNxN).name(), 0, 0, &s) << std::endl;
#endif 
  std::cout << SizeOf<TypesNxN>::value << std::endl;

  return 0;
}
