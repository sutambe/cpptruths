#include <iostream>
#include <typeinfo>
#include <string>

#include "boost/mpl/vector.hpp"
#include "boost/mpl/accumulate.hpp"
#include "boost/mpl/copy.hpp"
#include "boost/mpl/copy_if.hpp"
#include "boost/mpl/back_inserter.hpp"
#include "boost/mpl/size.hpp"
#include <boost/mpl/if.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/pop_back.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/int.hpp>
#include <boost/type_traits.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/contains.hpp>


using namespace boost;
using namespace boost::mpl;

template <class T, class U>
struct Merge
{
  //typedef typename copy <U, back_inserter<T> >::type type;
  
  typedef typename copy <T, back_inserter<vector<> > >::type type1;
  typedef typename copy <type1, back_inserter<U> >::type type;
  
  //typedef typename insert<T,typename end<T>::type, U>::type type;
};
typedef boost::mpl::vector0<> EmptyMPLVector;

template <class T = EmptyMPLVector >
struct EmptyVector
{
  enum { value = boost::mpl::equal<T, EmptyMPLVector >::type::value };
  typedef typename if_c<value, EmptyMPLVector, T>::type type;
};

template <class V1, class V2>
struct Intersection
{
  typedef typename front<V2>::type Head;
  typedef typename pop_front<V2>::type Tail;
  typedef typename if_<typename contains<V1, Head>::type,
                       typename push_back<typename Intersection<V1, Tail>::type, Head>::type,
                       typename Intersection<V1, typename EmptyVector<Tail>::type >::type>::type type;
};
template <class V1>
struct Intersection <V1, EmptyMPLVector >
{
  typedef EmptyMPLVector type;
};


struct PointerAdder
{
  template <class Vector, class T>
  struct apply
  {
    typedef mpl::vector< T* > Ptr;
    typedef typename Merge<Vector, Ptr>::type type; 
  };
};

int main(void)
{
  typedef mpl::vector<int, char, long> Integrals;
  typedef mpl::vector<int, float, double> Floats;
  //typedef mpl::push_back<Integrals, float>::type I1;
  //typedef mpl::push_back<I1, double>::type I2;
  
  typedef Merge<Integrals, Floats>::type Merged;
  typedef Intersection<Integrals, Floats>::type IS;

  std::cout << EmptyVector<vector<int> >::value << std::endl;
  std::cout << mpl::size<Merged>::value << std::endl;
  //std::cout << mpl::size<IS>::value << std::endl;
/*
  typedef mpl::vector<float,short,double,float,long,long double> types;
  typedef mpl::accumulate<
      types
    , int_<0>
    , if_ < is_float<_2>,next<_1>,_1 >
    >::type number_of_floats;

  BOOST_MPL_ASSERT_RELATION( number_of_floats::value, ==, 4 );

  typedef mpl::accumulate <types, mpl::vector<>, PointerAdder>::type PtrTypes;
  //Floats f = 10;
  //deref<begin<PtrTypes>::type >::type i = &f;
  
  //typedef mp::vector<float *,short *,double *,float *,long *,long double *> pointers;
  //BOOST_MPL_ASSERT(( mpl::equal<PtrTypes,pointers> ));
*/
  return 0;
}
