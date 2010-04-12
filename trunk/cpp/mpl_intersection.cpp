/* TO compile:
 *
 * time g++ mpl_intersection.cpp -o mpl_intersection -Wall -DIMPL_STYLE=1 -std=c++0x -I /project/mySVN/variadic_mpl
 *
 */


#include <iostream>
#include <typeinfo>

#define MY_STYLE 0
#define BOOST_STYLE 1
//#define IMPL_STYLE MY_STYLE
//#define IMPL_STYLE BOOST_STYLE

#ifdef __GXX_EXPERIMENTAL_CXX0X__ 

#if(IMPL_STYLE == MY_STYLE)
namespace boost { 
  namespace mpl {

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

/*
struct base_one { enum { value = 1 }; };
struct derived_zero : base_one { enum { value = 0 }; };

template< typename A, typename B >
struct type_equal {
   typedef derived_zero type;
};

template< typename A >
struct type_equal< A, A > {
   typedef base_one type;
};

template< typename Key, typename... Types >
struct pack_any {
   enum { value =
          std::common_type< typename type_equal< Key, Types >::type ... >::type::value 
        };
};

template <typename Vector, typename What> struct contains;

template <typename What, typename... Args>
struct contains < vector<Args...>, What>
{
  typedef contains type;
  enum { value = pack_any< What, Args...>::value };
};
*/
} // namespace mpl
} // namespace boost
#endif // IMPL_STYLE == MY_STYLE
#if(IMPL_STYLE == BOOST_STYLE)

#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_MPL_LIMIT_VECTOR_SIZE 50

// The mpl/vector.hpp include must come from variadic MPL library.
#include "boost/mpl/vector.hpp"

#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/copy_if.hpp>

#endif // IMPL_STYLE == BOOST_STYLE
#else // __GXX_EXPERIMENTAL_CXX0X__ 

#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_MPL_LIMIT_VECTOR_SIZE 50

#include <boost/mpl/vector.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/copy_if.hpp>

#endif // __GXX_EXPERIMENTAL_CXX0X__ 

#if(IMPL_STYLE == BOOST_STYLE)

template <class V1, class V2>
struct Intersection
{
  typedef typename 
    boost::mpl::copy_if<V1, boost::mpl::contains<V2, boost::mpl::placeholders::_1> >::type type;
};

#endif // IMPL_STYLE == BOOST_STYLE

#if(IMPL_STYLE == MY_STYLE)
template <class V1, class V2>
struct Intersection;

template <class V1, class V2, unsigned int N>
struct Intersection_impl
{
  typedef typename boost::mpl::front<V2>::type Head;
  typedef typename boost::mpl::pop_front<V2>::type Tail;
  typedef typename Intersection<V1, Tail>::type I;

#ifdef __GXX_EXPERIMENTAL_CXX0X__ 
  typedef typename 
    std::conditional<boost::mpl::contains<V1, Head>::value,
                     typename boost::mpl::push_back<I, Head>::type,
                     I >::type type;
#else
  typedef typename 
    boost::mpl::if_c<boost::mpl::contains<V1, Head>::value,
                     typename boost::mpl::push_back<I, Head>::type,
                     I >::type type;
#endif // __GXX_EXPERIMENTAL_CXX0X__ 
};

template <class V1, class V2>
struct Intersection_impl <V1, V2, 0>
{
  typedef boost::mpl::vector<> type;
};

template <class V1, class V2>
struct Intersection
{
  typedef typename Intersection_impl<V1, V2, 
          boost::mpl::size<V1>::value * boost::mpl::size<V2>::value>::type type;
};

#endif // IMPL_STYLE == MY_STYLE 

template <class V, unsigned N>
struct Print_impl
{
  static void show()
  {
    typedef typename boost::mpl::front<V>::type Head;
    typedef typename boost::mpl::pop_front<V>::type Tail;
    std::cout << typeid(Head).name() << std::endl;
    Print_impl<Tail, boost::mpl::size<Tail>::value>::show();
  }
};

template <class V>
struct Print_impl<V, 0>
{
  static void show()
  {
    std::cout << "END" << std::endl;
  }
};

template <class V>
struct Print
{
  static void show()
  {
    Print_impl<V, boost::mpl::size<V>::value>::show();
  }
};

template <unsigned int N> struct A {};
template <unsigned int N> struct B {};

#ifdef TEST10
int ten()
{
  typedef
    boost::mpl::vector < A<1> , A<2> , A<3> , A<4> , A<5> , A<6> , A<7> , A<8> , A<9> , A<10> > VectorA;

  typedef 
    boost::mpl::vector < B<1> , B<2> , B<3> , B<4> , B<5> , B<6> , B<7> , B<8> , B<9> , B<10> > VectorB;

  typedef Intersection <VectorA, VectorB>::type IS;
  return boost::mpl::size<IS>::value;
}
#endif

#ifdef TEST20
int twenty()
{
  typedef
    boost::mpl::vector < A<1> , A<2> , A<3> , A<4> , A<5> , A<6> , A<7> , A<8> , A<9> , A<10>,
                         A<11>, A<12>, A<13>, A<14>, A<15>, A<16>, A<17>, A<18>, A<19>, A<20> > VectorA;

  typedef 
    boost::mpl::vector < B<1> , B<2> , B<3> , B<4> , B<5> , B<6> , B<7> , B<8> , B<9> , B<10>,
                         B<11>, B<12>, B<13>, B<14>, B<15>, B<16>, A<17>, B<18>, B<19>, B<20> > VectorB;

  typedef Intersection <VectorA, VectorB>::type IS;
  return boost::mpl::size<IS>::value;
}
#endif

#ifdef TEST30
int thirty()
{
  typedef
    boost::mpl::vector < A<1> , A<2> , A<3> , A<4> , A<5> , A<6> , A<7> , A<8> , A<9> , A<10>,
                         A<11>, A<12>, A<13>, A<14>, A<15>, A<16>, A<17>, A<18>, A<19>, A<20>,
                         A<21>, A<22>, A<23>, A<24>, A<25>, A<26>, A<27>, A<28>, A<29>, A<30> > VectorA;

  typedef 
    boost::mpl::vector < B<1> , B<2> , B<3> , B<4> , B<5> , B<6> , B<7> , B<8> , B<9> , B<10>,
                         B<11>, B<12>, B<13>, B<14>, B<15>, B<16>, A<17>, B<18>, B<19>, B<20>,
                         B<21>, B<22>, B<23>, B<24>, B<25>, B<26>, B<27>, B<28>, B<29>, B<30> > VectorB;

  typedef Intersection <VectorA, VectorB>::type IS;
  return boost::mpl::size<IS>::value;
}
#endif

#ifdef TEST40
int forty()
{
  typedef
    boost::mpl::vector < A<1> , A<2> , A<3> , A<4> , A<5> , A<6> , A<7> , A<8> , A<9> , A<10>,
                         A<11>, A<12>, A<13>, A<14>, A<15>, A<16>, A<17>, A<18>, A<19>, A<20>,
                         A<21>, A<22>, A<23>, A<24>, A<25>, A<26>, A<27>, A<28>, A<29>, A<30>,
                         A<31>, A<32>, A<33>, A<34>, A<35>, A<36>, A<37>, A<38>, A<39>, A<40> > VectorA;

  typedef 
    boost::mpl::vector < B<1> , B<2> , B<3> , B<4> , B<5> , B<6> , B<7> , B<8> , B<9> , B<10>,
                         B<11>, B<12>, B<13>, B<14>, B<15>, B<16>, A<17>, B<18>, B<19>, B<20>,
                         B<21>, B<22>, B<23>, B<24>, B<25>, B<26>, B<27>, B<28>, B<29>, B<30>,
                         B<31>, B<32>, B<33>, B<34>, B<35>, B<36>, B<37>, A<38>, B<39>, B<40> > VectorB;

  typedef Intersection <VectorA, VectorB>::type IS;
  return boost::mpl::size<IS>::value;
}
#endif

#ifdef TEST50
int fifty()
{
  typedef
    boost::mpl::vector < A<1> , A<2> , A<3> , A<4> , A<5> , A<6> , A<7> , A<8> , A<9> , A<10>,
                         A<11>, A<12>, A<13>, A<14>, A<15>, A<16>, A<17>, A<18>, A<19>, A<20>,
                         A<21>, A<22>, A<23>, A<24>, A<25>, A<26>, A<27>, A<28>, A<29>, A<30>,
                         A<31>, A<32>, A<33>, A<34>, A<35>, A<36>, A<37>, A<38>, A<39>, A<40>,
                         A<41>, A<42>, A<43>, A<44>, A<45>, A<46>, A<47>, A<48>, A<49>, A<50> > VectorA;

  typedef 
    boost::mpl::vector < B<1> , B<2> , B<3> , B<4> , B<5> , B<6> , B<7> , B<8> , B<9> , B<10>,
                         B<11>, B<12>, B<13>, B<14>, B<15>, B<16>, A<17>, B<18>, B<19>, B<20>,
                         B<21>, B<22>, B<23>, B<24>, B<25>, B<26>, B<27>, B<28>, B<29>, B<30>,
                         B<31>, B<32>, B<33>, B<34>, B<35>, B<36>, B<37>, A<38>, B<39>, B<40>,
                         B<41>, B<42>, B<43>, B<44>, B<45>, B<46>, B<47>, B<48>, B<49>, B<50> > VectorB;

  typedef Intersection <VectorA, VectorB>::type IS;
  return boost::mpl::size<IS>::value;
}
#endif

int main(void)
{
  int (*func)() = 0;
#ifdef TEST10
  func = &ten;
#elif TEST20
  func = &twenty;
#elif TEST30
  func = &thirty;
#elif TEST40
  func = &forty;
#elif TEST50
  func = &fifty;
#endif

  std::cout << "SIZE = " << ((func)? func() : -1) << std::endl;

  return 0;
}

