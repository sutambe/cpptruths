#include <vector>
#include <string>
#include <utility>
#include <cstdio>
#include <array>
#include <complex>
#include <map>
#include <tuple>

template<typename... T>
struct is_noexcept_swappable
{
  typedef std::tuple<T...> Tuple;
  static Tuple *t; 
  //enum { value = noexcept(std::swap(std::declval<aggregate &>(), std::declval<aggregate &>())) };
  enum { value = noexcept(t->swap(std::declval<Tuple &>())) };
};

namespace nm {

struct base 
{
  void swap(base &) {}
};

void swap(base &, base &) noexcept {}

struct test : base
{
  std::string str;
  std::complex<double> cd;
  std::map<int, std::string> m;


  test() : str(), cd(), m() {}
  test(int i) : str("i") {}
  test(const test & t) 
    : str(t.str), cd(t.cd), m(t.m)
  {
      printf("%s\n", __PRETTY_FUNCTION__);
  }

  // test(test && t) // depends on whether member's move-ctor throws or not.

  
  test(test && t) noexcept(noexcept(std::string(std::declval<std::string>())))
    : str(std::move(t.str)), cd(std::move(t.cd)), m(std::move(t.m))
  {
    printf("%s\n", __PRETTY_FUNCTION__);
  }
 
  /*
  void swap(test & t2) noexcept(noexcept(swap(std::declval<std::string &>(), 
                                              std::declval<std::string &>())) &&
                                noexcept(swap(std::declval<std::complex<double> &>(), 
                                              std::declval<std::complex<double> &>())) &&
                                noexcept(swap(std::declval<std::map<int, std::string> &>(), 
                                              std::declval<std::map<int, std::string> &>())))
  */
  //void swap (test & t2) noexcept
  void swap (test & t2) noexcept(is_noexcept_swappable<base, std::complex<double>/*, std::map<int, std::string>*/ >::value)
  {
    using std::swap;
    base::swap(*this);
    swap(str, t2.str);
    swap(cd, t2.cd);
    swap(m, t2.m);
  }

  //~test () noexcept {}
};

void swap(test & t1, test &t2) noexcept(noexcept(t1.swap(t2)))
{
  t1.swap(t2);
}

} // namespce nm

namespace std {
template <>
  inline void swap(nm::test & t1, nm::test & t2) noexcept(noexcept(t1.swap(t2)))
  {
    t1.swap(t2); 
  }
}

int main(void)
{
  
  nm::test t;
  std::vector<nm::test> v{5}; // uses initializer_list only if nm::test has a int constructor, otherwise creates 5 nm::test objects.
  v.reserve(20);

  if(std::is_nothrow_move_constructible<nm::test>::value)
    printf("test move constructor does not throw.\n");
  else
    printf("test move constructor throws.\n");

  if(noexcept(t.swap(t)))
    printf("test::swap(test &) does not throw exception.\n");
  else
    printf("test::swap(test &) throws exception.\n");

  typedef std::pair<int, nm::test> Pair;
  Pair p1, p2;
  if(noexcept(swap(p1, p2)))
    printf("swap(pair &, pair&) does not throw exception.\n");
  else
    printf("swap(pair &, pair&) throws exception.\n");

  p1 = p2;

  std::array<nm::test, 5> test_array1, test_array2;

  test_array1.swap(test_array2);
  //swap(t, t);

  /*
  if(std::is_nothrow_destructible<nm::test>::value)
    printf("test destructor does not throw.\n");
  else
    printf("test destructor throws.\n");
  */

  return 0;
}
