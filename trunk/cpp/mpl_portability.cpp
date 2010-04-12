#include <typeinfo>
#include <iostream>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/version.hpp>
#include <boost/type_traits.hpp>

using namespace boost::mpl;
using namespace boost;

typedef vector<> EmptyVectorA;
typedef vector0<> EmptyVectorB;


#define SOL3

#if(defined SOL1 || defined SOL2)
template <class T>
void print_type(T)
{
  std::cout << typeid(T).name() << std::endl;
}
#elif(defined SOL3)

struct print_type_functor
{
  template <class T>
  void operator ()(T)
  {
    std::cout << typeid(T).name() << std::endl;    
  }
};

#endif //SOL3


#if(defined SOL1)

void iterate(vector0<>) { }
//void iterate(vector<>) { }

template <class V>
void iterate(V)
{
  print_type(typename front<V>::type());
  iterate(typename pop_front<V>::type());
}

#elif(defined SOL2)

 // Two base (primary) templates for iterate. They are 'same' signature-wise but
 // only one is active at any point. 
template <class V>
typename enable_if<empty<V>, void>::type
iterate(V) { }

template <class V>
typename disable_if<empty<V>, void>::type
iterate(V)
{
    print_type(typename front<V>::type());
    iterate(typename pop_front<V>::type());
}

#endif // SOL2





int main(int argc, char* argv[])
{
  typedef pop_front<EmptyVectorA>::type DoubleEmpty1;  // Fails on VS2008 but works on g++ 4.4.3
  typedef pop_front<EmptyVectorB>::type DoubleEmpty2;  // Fails on VS2008 but works on g++ 4.4.3
  std::cout << "is_same<DoubleEmpty1, DoubleEmpty2>::value = " 
            << is_same<DoubleEmpty1, DoubleEmpty2>::value << std::endl;
  
  typedef vector<float, double, long double> Floating;
  // 1 on g++, 0 on VS2008
  std::cout << "is_base_of<vector0<>, Floating> = " << boost::is_base_of<vector0<>, Floating>::value << std::endl;

#if(defined SOL1 || defined SOL2)
  iterate(Floating());
#elif(defined SOL3)
  boost::mpl::for_each<Floating>(print_type_functor());
#endif // SOL3

  std::cout << BOOST_LIB_VERSION << std::endl;
	return 0;
}


