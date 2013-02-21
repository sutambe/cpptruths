#include <iostream>
#include <ostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <type_traits>

namespace track {

class Radar { };
class Missile { };

std::ostream & print(std::ostream & o, const Radar &r)
{
  o << "This is an automatically generated Radar printer.\n";
  return o;
}

std::ostream & print(std::ostream & o, const Missile &m)
{
  o << "This is an automatically generated Missile printer.\n";
  return o;
}

}

namespace common {

template <class T>
struct is_basic { enum { value = 0 }; };

template <>
struct is_basic<uint32_t> { enum { value = 1 }; };

template <class T>
std::ostream & operator << (std::ostream & o, const T &t)
{
  return print(o, t);
}

template <class T>
typename std::enable_if<is_basic<T>::value, std::ostream &>::type
operator << (std::ostream & o, const std::vector<T> & v)
{
  std::copy(v.begin(), v.end(), std::ostream_iterator<T>(o, " "));
  return o;
}


}

namespace track {

std::ostream & operator << (std::ostream & o, const Radar & r)
{
  o << "This is user-defined operator << \n";
  return o;
}
}

namespace A
{
  typedef uint32_t AType;

  typedef std::vector<AType> ATypeSeq;
/*
  std::ostream& operator <<(std::ostream& os, const ATypeSeq& seq)
  {
    return os << "ATypeSeq";
  }
*/
  struct AStruct {};

  typedef std::vector<AStruct> AStructSeq;

  std::ostream& operator <<(std::ostream& os, const AStructSeq& seq)
  {
    return os << "AStructSeq";
  }

  enum class AEnum : uint32_t
  {
    ONE, TWO
  };

  std::ostream& operator <<(std::ostream& os, const AEnum& seq)
  {
    return os << "AEnum";
  }
};

namespace B
{
  typedef uint32_t BType;

  typedef std::vector<BType> BTypeSeq;
/*
  std::ostream& operator <<(std::ostream& os, const BTypeSeq& seq)
  {
    return os << "BTypeSeq";
  }
*/
  struct BStruct {};

  typedef std::vector<BStruct> BStructSeq;

  std::ostream& operator <<(std::ostream& os, const BStructSeq& seq)
  {
    return os << "BStructSeq";
  }
/*
  typedef std::vector<A::AStruct> BAStructSeq;

  std::ostream& operator <<(std::ostream& os, const BAStructSeq& seq)
  {
    return os << "BAStructSeq";
  }
*/
};

int main(int, char*[])
{
  using namespace common;

  track::Radar r;
  track::Missile m;
  std::cout << r << m;

  //using namespace A;
  
  int array[] = { 10, 20, 30 };

  A::ATypeSeq  a_seq(array, array + 3);
  std::cout << a_seq << std::endl;

  B::BTypeSeq  b_seq(array, array + 3);
  std::cout << b_seq << std::endl;

  A::AStructSeq a_sseq;
  std::cout << a_sseq << std::endl;

  A::AEnum a_enum(A::AEnum::ONE);
  std::cout << a_enum << std::endl;

  B::BStructSeq b_sseq;
  std::cout << b_sseq << std::endl;

  //B::BAStructSeq ba_sseq;
  //std::cout << ba_sseq << std::endl;

  return 0;
}

