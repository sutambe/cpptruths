#include "boost/concept_check.hpp"
#include "boost/mpl/assert.hpp"
#include "boost/type_traits/is_same.hpp"


template <class T>
struct NoInt
{
  //BOOST_MPL_ASSERT_NOT((boost::is_same<T,int>));
  BOOST_CONCEPT_ASSERT_NOT((boost::Convertible<T,int>));
};

int main(void)
{
  NoInt<int> x;
  return 0;
}

