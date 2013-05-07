/*
 * g++49 constexpr-strfind.cpp -o constexpr-strfind -Wall -std=c++11 -DTEXT=m2 -DMAXIMUM=2
 */

#include <cstdio>

#define TO_STR_IMPL(R) #R
#define TO_STR(R) TO_STR_IMPL(R)

#define MEMBER_NAME(N,STR)                   \
template <>                                  \
struct member_name<N> {                      \
  static constexpr const char *str = STR;    \
};                                           \
constexpr const char *member_name<N>::str; 

template <unsigned N>
struct member_name;

MEMBER_NAME(0, "m0");
MEMBER_NAME(1, "m1");
MEMBER_NAME(2, "m2");

constexpr bool strmatch(const char *s1, const char *s2)
{
  return (*s1==*s2) ? 
            (*s1=='\0') ? 
               true
               : strmatch(s1+1, s2+1) 
            : false;
}

template <unsigned I, unsigned MAX>
struct Finder 
{
  static constexpr int where(const char *s)
  {
    return strmatch(s, member_name<I>::str)? I : Finder<I+1, MAX>::where(s);
  }
};

template <unsigned MAX>
struct Finder<MAX, MAX> 
{
  static constexpr int where(const char *s)
  {
    return strmatch(s, member_name<MAX>::str)? MAX : -1;
  }
};

template <unsigned MAX>
constexpr int find(const char *s) 
{
  return Finder<0, MAX>::where(s);
}

int main(void)
{
  //static_assert(strmatch(TO_STR(TEXT), member_name<N>::str), "Does not match");
  static_assert(find<MAXIMUM>(TO_STR(TEXT)) >= 0, "not found!");
  static_assert(find<MAXIMUM>(TO_STR(TEXT)) <= MAXIMUM, "not found!");
  printf("index = %d\n", find<MAXIMUM>(TO_STR(TEXT)));

  return 0;
}
