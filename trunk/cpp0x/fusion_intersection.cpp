/* TO compile:
 *
 * time g++ mpl_intersection.cpp -o mpl_intersection -Wall -DIMPL_STYLE=1 -DTEST50 -std=c++0x -I /project/mySVN/variadic_mpl
 *
 */


#include <iostream>
#include <typeinfo>

#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_MPL_LIMIT_VECTOR_SIZE 50

#ifdef USE_MPL

#include <boost/mpl/vector.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/copy_if.hpp>
#include <boost/mpl/count_if.hpp>

using boost::mpl::vector;
using boost::mpl::front;
using boost::mpl::pop_front;
using boost::mpl::push_back;
using boost::mpl::empty;
using boost::mpl::size;
using boost::mpl::count_if;
using boost::mpl::copy_if;
using boost::mpl::placeholders::_1;

#endif // USE_MPL

#ifdef USE_FUSION

#define FUSION_MAX_VECTOR_SIZE 50
#include <boost/fusion/container/vector.hpp>

#include <boost/fusion/sequence/intrinsic/front.hpp>
#include <boost/fusion/include/front.hpp>
#include <boost/fusion/include/filter_if.hpp>
#include <boost/fusion/include/count.hpp>
#include <boost/fusion/include/pop_front.hpp>
#include <boost/fusion/include/push_back.hpp>
#include <boost/fusion/include/size.hpp>


using boost::fusion::vector;
using boost::fusion::filter_if;
using boost::fusion::pop_front;
using boost::fusion::size;
using boost::fusion::count;
using boost::fusion::front;
using boost::fusion::push_back;

#endif // USE_FUSION

template <class V1, class V2>
struct Intersection;

template <class V1, class V2, unsigned int N>
struct Intersection_impl
{
  typedef typename front<V2>::type Head;
  typedef typename pop_front<V2>::type Tail;
  typedef typename Intersection<V1, Tail>::type I;

#ifdef __GXX_EXPERIMENTAL_CXX0X__ 
  typedef typename 
    std::conditional<count<V1, Head>::value,
                     typename push_back<I, Head>::type,
                     I> ::type type;
#else
  typedef typename 
    if_<count<V1, Head>,
        typename push_back<I, Head>::type,
        I >::type type;
#endif // __GXX_EXPERIMENTAL_CXX0X__ 
};

template <class V1, class V2>
struct Intersection_impl <V1, V2, 0>
{
  typedef vector<> type;
};

template <class V1, class V2>
struct Intersection
{
  typedef typename 
    Intersection_impl<V1, V2,
                      size<V1>::value * size<V2>::value>::type type;
};



template <class V, unsigned N>
struct Print_impl
{
  static void show()
  {
    typedef typename front<V>::type Head;
    typedef typename pop_front<V>::type Tail;
    std::cout << typeid(Head).name() << std::endl;
    Print_impl<Tail, size<Tail>::value>::show();
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
    Print_impl<V, size<V>::value>::show();
  }
};

template <unsigned int N> struct A {};
template <unsigned int N> struct B {};

#ifdef TEST10
int ten()
{
  typedef
    vector < A<1> , A<2> , A<3> , A<4> , A<5> , A<6> , A<7> , A<8> , A<9> , A<10> > VectorA;

  typedef 
    vector < B<1> , B<2> , B<3> , B<4> , B<5> , B<6> , B<7> , B<8> , B<9> , B<10> > VectorB;

  typedef Intersection <VectorA, VectorB>::type IS;
  return size<IS>::value;
}
#endif

#ifdef TEST20
int twenty()
{
  typedef
    vector < A<1> , A<2> , A<3> , A<4> , A<5> , A<6> , A<7> , A<8> , A<9> , A<10>,
                         A<11>, A<12>, A<13>, A<14>, A<15>, A<16>, A<17>, A<18>, A<19>, A<20> > VectorA;

  typedef 
    vector < B<1> , B<2> , B<3> , B<4> , B<5> , B<6> , B<7> , B<8> , B<9> , B<10>,
                         B<11>, B<12>, B<13>, B<14>, B<15>, B<16>, A<17>, B<18>, B<19>, B<20> > VectorB;

  typedef Intersection <VectorA, VectorB>::type IS;
  return size<IS>::value;
}
#endif

#ifdef TEST30
int thirty()
{
  typedef
    vector < A<1> , A<2> , A<3> , A<4> , A<5> , A<6> , A<7> , A<8> , A<9> , A<10>,
                         A<11>, A<12>, A<13>, A<14>, A<15>, A<16>, A<17>, A<18>, A<19>, A<20>,
                         A<21>, A<22>, A<23>, A<24>, A<25>, A<26>, A<27>, A<28>, A<29>, A<30> > VectorA;

  typedef 
    vector < B<1> , B<2> , B<3> , B<4> , B<5> , B<6> , B<7> , B<8> , B<9> , B<10>,
                         B<11>, B<12>, B<13>, B<14>, B<15>, B<16>, A<17>, B<18>, B<19>, B<20>,
                         B<21>, B<22>, B<23>, B<24>, B<25>, B<26>, B<27>, B<28>, B<29>, B<30> > VectorB;

  typedef Intersection <VectorA, VectorB>::type IS;
  return size<IS>::value;
}
#endif

#ifdef TEST40
int forty()
{
  typedef
    vector < A<1> , A<2> , A<3> , A<4> , A<5> , A<6> , A<7> , A<8> , A<9> , A<10>,
                         A<11>, A<12>, A<13>, A<14>, A<15>, A<16>, A<17>, A<18>, A<19>, A<20>,
                         A<21>, A<22>, A<23>, A<24>, A<25>, A<26>, A<27>, A<28>, A<29>, A<30>,
                         A<31>, A<32>, A<33>, A<34>, A<35>, A<36>, A<37>, A<38>, A<39>, A<40> > VectorA;

  typedef 
    vector < B<1> , B<2> , B<3> , B<4> , B<5> , B<6> , B<7> , B<8> , B<9> , B<10>,
                         B<11>, B<12>, B<13>, B<14>, B<15>, B<16>, A<17>, B<18>, B<19>, B<20>,
                         B<21>, B<22>, B<23>, B<24>, B<25>, B<26>, B<27>, B<28>, B<29>, B<30>,
                         B<31>, B<32>, B<33>, B<34>, B<35>, B<36>, B<37>, A<38>, B<39>, B<40> > VectorB;

  typedef Intersection <VectorA, VectorB>::type IS;
  return size<IS>::value;
}
#endif

#ifdef TEST50
int fifty()
{
  typedef
    vector < A<1> , A<2> , A<3> , A<4> , A<5> , A<6> , A<7> , A<8> , A<9> , A<10>,
                         A<11>, A<12>, A<13>, A<14>, A<15>, A<16>, A<17>, A<18>, A<19>, A<20>,
                         A<21>, A<22>, A<23>, A<24>, A<25>, A<26>, A<27>, A<28>, A<29>, A<30>,
                         A<31>, A<32>, A<33>, A<34>, A<35>, A<36>, A<37>, A<38>, A<39>, A<40>,
                         A<41>, A<42>, A<43>, A<44>, A<45>, A<46>, A<47>, A<48>, A<49>, A<50> > VectorA;

  typedef 
    vector < B<1> , B<2> , B<3> , B<4> , B<5> , B<6> , B<7> , B<8> , B<9> , B<10>,
                         B<11>, B<12>, B<13>, B<14>, B<15>, B<16>, A<17>, B<18>, B<19>, B<20>,
                         B<21>, B<22>, B<23>, B<24>, B<25>, B<26>, B<27>, B<28>, B<29>, B<30>,
                         B<31>, B<32>, B<33>, B<34>, B<35>, B<36>, B<37>, A<38>, B<39>, B<40>,
                         B<41>, B<42>, B<43>, B<44>, B<45>, B<46>, B<47>, B<48>, B<49>, B<50> > VectorB;

  typedef Intersection <VectorA, VectorB>::type IS;
  return size<IS>::value;
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

