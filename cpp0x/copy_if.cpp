/*
  time g++45 copy_if.cpp -o copy_if -Wall -W -DTEST100 -std=c++0x
*/


#include <iostream>

#ifdef  __GXX_EXPERIMENTAL_CXX0X__ 

#include <typeinfo>

struct null_type {};

template <typename... Arg>
struct vector {};

template <typename V> struct front;
template <typename V> struct pop_front;

template <typename Head, typename... Tail>
struct front <vector <Head, Tail...> >
{
  typedef Head type;
};

template <>
struct front <vector <> >
{
  typedef null_type type;
};

template <typename Head, typename... Tail>
struct pop_front <vector <Head, Tail...> >
{
  typedef vector<Tail...> type;
};

template <>
struct pop_front <vector <> >
{
  typedef vector<> type;
};

template <typename Vector, typename T> struct push_back;

template <typename T, typename... Args>
struct push_back < vector<Args...>, T>
{
  typedef vector<Args..., T> type;
};

template <typename Vector> struct size;

template <typename... Args>
struct size <vector <Args...> >
{
  typedef size type;
  enum { value = sizeof...(Args) };
};

template <typename Vector, typename What> struct contains;

template <typename What, typename Head, typename... Tail>
struct contains < vector<Head, Tail...>, What> : 
  std::conditional < std::is_same<Head, What>::value,
                     std::true_type,
                     contains < vector<Tail...>, What> >::type
{
  typedef contains type;
};

template <typename What>
struct contains <vector<>, What>
{
  typedef contains type;
  enum { value = 0 };
};

template <class V1, template <typename> class Pred>
struct copy_if
{
  typedef typename front<V1>::type Head;
  typedef typename pop_front<V1>::type Tail;
  typedef typename std::conditional<
                      Pred<Head>::value,
                      typename push_back<typename copy_if<Tail, Pred>::type,
                                         Head
                                        >::type,
                      typename copy_if<Tail, Pred>::type>::type type;
};

template <template <typename> class Pred>
struct copy_if <vector<>, Pred>
{
  typedef vector<> type;
};

template <class Vector>
struct Inside
{
  template <class What>
  struct IsPresent
  {
    typedef IsPresent type;
    enum { value = contains<Vector, What>::value };
  };
};

template <class V1, class V2>
struct Intersection
{
  typedef typename copy_if<V1, Inside<V2>::template IsPresent>::type type;
};

#define MPL_VECTOR_N(N,...) vector<__VA_ARGS__>

#else // __GXX_EXPERIMENTAL_CXX0X__

#include <boost/mpl/copy_if.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/size.hpp>

template <class V1, class V2>
struct Intersection
{
  typedef typename 
    boost::mpl::copy_if<V1, boost::mpl::contains<V2, boost::mpl::placeholders::_1> >::type type;
};

#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_MPL_LIMIT_VECTOR_SIZE 50
#define GENERATE_MPL_VECTOR_SIZE 100

#include <boost/mpl/vector.hpp>

#if (GENERATE_MPL_VECTOR_SIZE > BOOST_MPL_LIMIT_VECTOR_SIZE)

#include "boost/mpl/aux_/config/ctps.hpp"
#include "boost/preprocessor/iterate.hpp"

#warning Started generating mpl vectors

namespace boost {
namespace mpl {
    #define BOOST_PP_ITERATION_PARAMS_1 (3,( \
        BOOST_PP_INC(BOOST_MPL_LIMIT_VECTOR_SIZE), \
        GENERATE_MPL_VECTOR_SIZE, \
        "boost/mpl/vector/aux_/numbered.hpp"))
    #include BOOST_PP_ITERATE()
}
}
#warning Finished generating mpl vectors

#endif // GENERATE_MPL_VECTOR_SIZE

using boost::mpl::size;

#define MPL_VECTOR_N(N,...) boost::mpl::vector##N < __VA_ARGS__ >

#endif // __GXX_EXPERIMENTAL_CXX0X__

template <unsigned> struct A {};
template <unsigned> struct B {};

#ifdef TEST50
int fifty()
{
  typedef MPL_VECTOR_N(50, A<1> , A<2> , A<3> , A<4> , A<5> , A<6> , A<7> , A<8> , A<9> , A<10>,
                           A<11>, A<12>, A<13>, A<14>, A<15>, A<16>, A<17>, A<18>, A<19>, A<20>,
                           A<21>, A<22>, A<23>, A<24>, A<25>, A<26>, A<27>, A<28>, A<29>, A<30>,
                           A<31>, A<32>, A<33>, A<34>, A<35>, A<36>, A<37>, A<38>, A<39>, A<40>,
                           A<41>, A<42>, A<43>, A<44>, A<45>, A<46>, A<47>, A<48>, A<49>, A<50>) VectorA;

  typedef MPL_VECTOR_N(50, B<1> , B<2> , B<3> , B<4> , B<5> , B<6> , B<7> , B<8> , B<9> , B<10>,
                           B<11>, B<12>, B<13>, B<14>, B<15>, B<16>, A<17>, B<18>, B<19>, B<20>,
                           B<21>, B<22>, B<23>, B<24>, B<25>, B<26>, B<27>, B<28>, B<29>, B<30>,
                           B<31>, B<32>, B<33>, B<34>, B<35>, B<36>, B<37>, A<38>, B<39>, B<40>,
                           B<41>, B<42>, B<43>, B<44>, B<45>, B<46>, B<47>, B<48>, B<49>, B<50>) VectorB;

  typedef Intersection <VectorA, VectorB>::type IS;
  return size<IS>::value;
}
#endif

#ifdef TEST100
int hundred()
{
  typedef MPL_VECTOR_N(100, A<1> , A<2> , A<3> , A<4> , A<5> , A<6> , A<7> , A<8> , A<9> , A<10>,
                            A<11>, A<12>, A<13>, A<14>, A<15>, A<16>, A<17>, A<18>, A<19>, A<20>,
                            A<21>, A<22>, A<23>, A<24>, A<25>, A<26>, A<27>, A<28>, A<29>, A<30>,
                            A<31>, A<32>, A<33>, A<34>, A<35>, A<36>, A<37>, A<38>, A<39>, A<40>,
                            A<41>, A<42>, A<43>, A<44>, A<45>, A<46>, A<47>, A<48>, A<49>, A<50>,
                            A<51>, A<52>, A<53>, A<54>, A<55>, A<56>, A<57>, A<58>, A<59>, A<60>,
                            A<61>, A<62>, A<63>, A<64>, A<65>, A<66>, A<67>, A<68>, A<69>, A<70>,
                            A<71>, A<72>, A<77>, A<74>, A<75>, A<76>, A<77>, A<78>, A<79>, A<80>,
                            A<81>, A<82>, A<83>, A<84>, A<85>, A<86>, A<87>, A<88>, A<89>, A<90>,
                            A<91>, A<92>, A<93>, A<94>, A<95>, A<96>, A<97>, A<98>, A<99>, A<100>)  VectorA;

  typedef MPL_VECTOR_N(100, B<1> , B<2> , B<3> , B<4> , B<5> , B<6> , B<7> , B<8> , B<9> , B<10>,
                            B<11>, B<12>, B<13>, B<14>, B<15>, B<16>, A<17>, B<18>, B<19>, B<20>,
                            B<21>, B<22>, B<23>, B<24>, B<25>, B<26>, B<27>, B<28>, B<29>, B<30>,
                            B<31>, B<32>, B<33>, B<34>, B<35>, B<36>, B<37>, A<38>, B<39>, B<40>,
                            B<41>, B<42>, B<43>, B<44>, B<45>, B<46>, B<47>, B<48>, B<49>, B<50>,
                            B<51>, B<52>, B<53>, B<54>, B<55>, B<56>, A<57>, B<58>, B<59>, B<20>,
                            B<61>, B<62>, B<63>, B<64>, B<65>, B<66>, B<67>, B<68>, B<69>, B<70>,
                            B<71>, B<72>, B<73>, B<74>, B<75>, B<76>, B<77>, A<78>, B<79>, B<80>,
                            B<81>, B<82>, B<83>, B<84>, B<85>, B<86>, B<87>, B<88>, B<89>, B<90>,
                            B<91>, B<92>, B<93>, B<94>, B<95>, B<96>, B<97>, B<98>, B<99>, B<100>) VectorB;

  typedef Intersection <VectorA, VectorB>::type IS;
  return size<IS>::value;
}
#endif //TEST100

int main(void)
{

#ifdef TEST100
  std::cout << hundred() << std::endl;
#elif TEST50
  std::cout << fifty() << std::endl;
#endif

}

