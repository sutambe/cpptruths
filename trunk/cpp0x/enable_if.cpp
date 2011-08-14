#include <utility>
#include <cstdio>
#include <vector>

template <class T>
struct is_iterator
{
  typedef is_iterator type;
  enum { value = 0 };
};

template <class T>
struct is_iterator<T *>
{
  typedef is_iterator<T *> type;
  enum { value = 1 };
};
/*
template <class T>
struct is_iterator<typename std::vector<T>::iterator>
{
  typedef is_iterator<typename std::vector<T>::iterator> type;
  enum { value = 1 };
};
*/
template <class T>
struct Foo
{
  // Default template parameters for function templates in C++0x only.
  template <class Iterator/*
            class = typename std::enable_if
                    <is_iterator<Iterator>::value>
                     ::type*/
    >
  Foo(Iterator, Iterator);

  Foo(T a, T b) {
    printf(__PRETTY_FUNCTION__); printf("\n");
  }

  template <class Blah>
  void bar (Blah, Blah, typename std::enable_if<!is_iterator<Blah>::value>::type * = 0) {
    printf(__PRETTY_FUNCTION__); printf("\n");
  }

  template <class Iterator>
  void bar (Iterator, Iterator, typename std::enable_if <is_iterator<Iterator>::value>::type * = 0) {
    printf(__PRETTY_FUNCTION__); printf("\n");
  }

};

template <class T>
template <class Iterator>//, class>
Foo<T>::Foo(Iterator i, Iterator j)
{  
  printf(__PRETTY_FUNCTION__); printf("\n");
}

int main(void)
{
  Foo<int> f_int(10, 20);
  
  std::vector<int> v;
  Foo<int> f_iter(v.begin(), v.end());

  f_int.bar(10, 20);
  f_int.bar(v.begin(), v.end());

  return 0;
}

