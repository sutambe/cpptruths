#define BOOST_TYPEOF_LIMIT_SIZE 200

#include "boost/mpl/vector.hpp"
#include "boost/mpl/size_t.hpp"
#include "boost/mpl/begin_end.hpp"
#include "boost/mpl/aux_/config/ctps.hpp"
#include "boost/preprocessor/iterate.hpp"
#include "boost/preprocessor/if.hpp"
#include "boost/preprocessor/repetition/enum.hpp"
#include "boost/config.hpp"

#if (BOOST_TYPEOF_LIMIT_SIZE > BOOST_MPL_LIMIT_VECTOR_SIZE)

#warning started generating

namespace boost
{
namespace mpl
{
    #define BOOST_PP_ITERATION_PARAMS_1 (3,( \
        BOOST_PP_INC(BOOST_MPL_LIMIT_VECTOR_SIZE), \
        BOOST_TYPEOF_LIMIT_SIZE, \
        "boost/mpl/vector/aux_/numbered.hpp"))
    #include BOOST_PP_ITERATE()
}
}

#warning done generating

#endif//BOOST_TYPEOF_LIMIT_SIZE > BOOST_MPL_LIMIT_VECTOR_SIZE

#define MACRO(z, n, _)\
boost::mpl::size_t<n>

#warning started getting begin
typedef boost::mpl::begin<BOOST_PP_CAT(boost::mpl::vector,
BOOST_TYPEOF_LIMIT_SIZE)<
BOOST_PP_ENUM(BOOST_TYPEOF_LIMIT_SIZE, MACRO, ~)
> >::type it;
#warning done getting begin

main()
{}
