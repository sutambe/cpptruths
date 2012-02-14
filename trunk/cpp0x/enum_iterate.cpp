#include <array>
#include <iostream>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/array/elem.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/control/if.hpp>

#define PP_NARG(...)        PP_NARG_IMPL(__VA_ARGS__,PP_RSEQ_N()) 
#define PP_NARG_IMPL(...)   PP_ARG_N(__VA_ARGS__) 
#define PP_ARG_N( \
          _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
         _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
         _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
         _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
         _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
         _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
         _61,_62,_63,N,...) N 
#define PP_RSEQ_N() \
         63,62,61,60,                   \
         59,58,57,56,55,54,53,52,51,50, \
         49,48,47,46,45,44,43,42,41,40, \
         39,38,37,36,35,34,33,32,31,30, \
         29,28,27,26,25,24,23,22,21,20, \
         19,18,17,16,15,14,13,12,11,10, \
         9,8,7,6,5,4,3,2,1,0 

//#define PP_MAKE_ARRAY( ... )  (PP_NARG(__VA_ARGS__) , ( __VA_ARGS__ ) )
#define PP_MAKE_ARRAY(arg1, ... )  BOOST_PP_IF(PP_NARG(__VA_ARGS__) == 1, (arg1), (arg1) PP_MAKE_ARRAY(__VA_ARGS__))

#define PP_FOREACH( MACRO, prefix, ... )  BOOST_PP_REPEAT( PP_NARG(__VA_ARGS__), MACRO, (prefix, PP_MAKE_ARRAY(__VA_ARGS__)))

template<class T>
struct EnumIterator;

#define ATTACH(prefix, array)  prefix::BOOST_PP_ARRAY_ELEM(n,data)

#define PREFIX_TYPE(z,n,data) ATTACH data

#define DEFINE_ENUM(name, underlying_type, ...) \
  enum class name : underlying_type {                  \
    __VA_ARGS__         \
  };                                                  \
 \
   template <>                                          \
  struct EnumIterator<name> {                          \
    static constexpr name array[] = { BOOST_PP_SEQ_FOR_EACH(PREFIX_TYPE, name, PP_MAKE_ARRAY(__VA_ARGS__)) \
        };\
    enum { LENGTH = sizeof(array)/sizeof(array[0]) }; \
    static constexpr const name * begin() { \
      return array;                    \
    }                                                  \
    static constexpr const name * end() { \
      return array + LENGTH;                      \
    }                                                  \
  };\
  constexpr name EnumIterator<name>::array[EnumIterator<name>::LENGTH];  

DEFINE_ENUM(Color, unsigned, Red, Orange, Yellow, Green, Blue, Indigo, Violet)

template <class T>
void print(T t)
{
  switch(t) {
    case Color::Red:     std::cout << "Color::Red\n";    break;
    case Color::Orange:  std::cout << "Color::Orange\n"; break;
    case Color::Yellow:  std::cout << "Color::Yellow\n"; break;
    case Color::Green:   std::cout << "Color::Red\n";    break;
    case Color::Blue:    std::cout << "Color::Blue\n";   break;
    case Color::Indigo:  std::cout << "Color::Indigo\n"; break;
    case Color::Violet:  std::cout << "Color::Violet\n"; break;
  }
}
int main(void)
{
  Color c = Color::Orange;
  for(auto i = EnumIterator<Color>::begin();i < EnumIterator<Color>::end(); ++i)
  {
    print(*i);
  }

  return 0;
}

