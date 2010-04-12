#include <iostream>

template <class T1, class T2>
struct is_same
{
  enum { value = false };
};

template <class T>
struct is_same <T, T>
{
  enum { value = true };
};

template <bool, bool b1, bool b2>
struct IF;

template <bool b1, bool b2>
struct IF <false, b1, b2>
{
  enum { value = b2 };
};

template <bool b1, bool b2>
struct IF <true, b1, b2>
{
  enum { value = b1 };
};

struct empty {};

template < typename T1=empty, typename T2=empty, typename T3=empty, 
           typename T4=empty, typename T5=empty > 
struct typelist;

template <typename T1, typename T2, typename T3, typename T4, typename T5> 
struct typelist
{
    typedef T1 head;
    typedef typelist <T2, T3, T4, T5> tail;
};

template <class List, class T>
struct contains 
{
  enum { value = IF<is_same<typename List::head,T>::value,
                    true, contains<typename List::tail, T>::value >::value }; 
};

template <class T>
struct contains <typelist<empty,empty,empty,empty,empty>, T> 
{
  enum { value = 0 }; 
};


int main (void)
{
  typedef typelist <int, long> MyList;
  std::cout << contains<MyList, int>::value << std::endl;
  std::cout << contains<MyList, float>::value << std::endl;

  return 0;
}
