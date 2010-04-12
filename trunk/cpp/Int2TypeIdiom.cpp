#include <iostream>
#include <tr1/array>

template <int i>
struct Int2Type
{
  typedef Int2Type<i> type;
  enum { value = i };
  //typedef typename Int2Type<i+1>::type Next;
  //typedef typename Int2Type<i-1>::type Previous;
  operator int () { return i; }
};


template <class T, unsigned int N>
class Array : public std::tr1::array <T, N>
{
     enum AlgoType { NOOP, INSERTION_SORT, QUICK_SORT };
     static const int algo = (N==0) ? NOOP : 
                             (N==1) ? NOOP :
                             (N<50) ? INSERTION_SORT : QUICK_SORT;
     void sort (Int2Type<NOOP>) { std::cout << "NOOP\n"; }
     void sort (Int2Type<INSERTION_SORT>) { std::cout << "INSERTION_SORT\n"; } 
     void sort (Int2Type<QUICK_SORT>) { std::cout << "QUICK_SORT\n"; } 
  public:
     void sort()
     {
       sort (Int2Type<algo>());
     }
};

int main(void)
{
  const int N=40;
  Array<int,N> a;
  a.sort();
  std::cout << sizeof(Array<int,1>) << std::endl;
  std::cout << sizeof(Array<int,2>) << std::endl;
  std::cout << sizeof(Array<int,3>) << std::endl;
  std::cout << sizeof(Array<int,4>) << std::endl;
  std::cout << sizeof(Array<int,5>) << std::endl;
  std::cout << std::tr1::tuple_size<std::tr1::array<int,5> >::value << std::endl;

  return 0; 
}
