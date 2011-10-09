#include "boost/utility/addressof.hpp"
#include <cstdio>

class nonaddressable {
public:

    nonaddressable( int = 0 )
    {
    }

    void dummy(); // Silence GCC warning: all member of class are private
    typedef double useless_type;

private:

    useless_type operator&() const;
};

int main(void)
{
  nonaddressable na;
  //nonaddressable * naptr = &na;
  nonaddressable * naptr = boost::addressof(na);
  nonaddressable * naptr2 =  
    reinterpret_cast<nonaddressable *>(& const_cast<char&>(reinterpret_cast<const volatile char &>(na)));
  printf("%s\n", (naptr == naptr2)? "Same" : "Different");

  return 0;
}

