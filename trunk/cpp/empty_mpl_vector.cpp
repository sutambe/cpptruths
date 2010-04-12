#include <boost/mpl/vector.hpp>
#include <boost/mpl/pop_back.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/type_traits.hpp>
#include <iostream>
#include <typeinfo>

using namespace std;
using namespace boost::mpl;

typedef boost::mpl::vector<>  EmptyMPLVector;
typedef boost::mpl::vector0<> EmptyMPLVector0;

void printType(EmptyMPLVector)
{
  cout << "vector<>" << endl;
}

void printType(EmptyMPLVector0)
{
  cout << "vector0<>" << endl;
}

template <class V>
void printType(V v)
{
  vector<> i = 10;
  std::cout << boost::is_same<vector0<>,V>::value << std::endl;
  std::cout << boost::is_same<vector<>,V>::value << std::endl;
  std::cout << boost::is_base_of<vector0<>,V>::value << std::endl;
  std::cout << boost::is_base_of<vector0<>,vector<> >::value << std::endl;
  std::cout << boost::is_base_of<vector<>,V >::value << std::endl;
  //typedef typename front<V>::type Head;
  //typedef typename pop_front<V>::type Tail;
  //cout << typeid(Head).name() << endl;
  //Tail tail;
  //printType(tail);
};

template <class V, unsigned N>
struct Print_impl
{
  static void show()
  {
    typedef typename front<V>::type Head;
    typedef typename pop_front<V>::type Tail;
    cout << typeid(Head).name() << endl;
    Print_impl<Tail, size<Tail>::value>::show();
  }
};

template <class V>
struct Print_impl<V, 0>
{
  static void show()
  {
    printType(V());
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


template <class V1, class V2>
struct Intersection;

template <class V1, class V2, unsigned int N>
struct Intersection_impl
{
  typedef typename front<V2>::type Head;
  typedef typename pop_front<V2>::type Tail;
  typedef typename Intersection<V1, Tail>::type I;
  typedef typename 
    if_<contains<V1, Head>,
        typename push_back<I, Head>::type,
        I
    >::type type;
};

template <class V1, class V2>
struct Intersection_impl <V1, V2, 0>
{
  typedef vector0<> type;
};

template <class V1, class V2>
struct Intersection
{
  typedef typename Intersection_impl<V1, V2, size<V1>::value * size<V2>::value>::type type;
};

int main (void)
{
  typedef vector<double, float> Floating;
  typedef pop_back<Floating>::type F;
  typedef pop_back<F>::type EmptyType;
  typedef pop_back<EmptyType>::type E;
  std::cout << boost::is_base_of<vector0<>, Floating>::value << std::endl;
  cout << "Empty = " << empty<EmptyType>::value << endl;
  cout << "Size = "  << size<EmptyType>::value << endl;

  typedef Intersection<Floating, vector<float> >::type VF;
  Print<Floating>::show();

  return 0;
}
