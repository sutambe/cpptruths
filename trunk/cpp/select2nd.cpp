#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include <iterator>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/foreach.hpp>

template <class Pair>
struct select2nd : std::unary_function<Pair, typename Pair::second_type>
{
  typedef std::unary_function<Pair, typename Pair::second_type> super;
  typedef typename super::result_type result_type;
  typedef typename super::argument_type argument_type;

  result_type & operator ()(argument_type & p) const {
    return p.second;
  }
  result_type const & operator ()(argument_type const & p) const {
    return p.second;
  }
};

template <class UnaryFunc1, class UnaryFunc2>
struct unary_compose : std::unary_function<typename UnaryFunc2::argument_type,
                                           typename UnaryFunc1::result_type>
{
  typedef std::unary_function<typename UnaryFunc2::argument_type, 
                              typename UnaryFunc1::result_type> super;
  typedef typename super::result_type result_type;
  typedef typename super::argument_type argument_type;

  UnaryFunc1 func1_;
  UnaryFunc2 func2_;
  unary_compose(UnaryFunc1 f1, UnaryFunc2 f2) : func1_(f1), func2_(f2) {}
  result_type operator () (argument_type arg) {
    return func1_(func2_(arg));
  }
};

template <class UnaryFunc1, class UnaryFunc2>
unary_compose<UnaryFunc1, UnaryFunc2>
compose1(UnaryFunc1 f1, UnaryFunc2 f2) {
  return unary_compose<UnaryFunc1, UnaryFunc2>(f1,f2);
};

typedef std::map<int, unsigned> mymap;
bool foo(mymap::value_type const &)
{
  return 1;
}

typedef std::pair<int, int> IntPair;
std::ostream & operator << (std::ostream & o, const IntPair & p)
{
  o << "[" << p.first << ", " << p.second << "]";
  return o;
}

int main(void) 
{
  mymap m;
  for(int i = 0; i < 5; ++i )
    m[i] = i;

  // 'remove' all elements from map where .second < 3
  //std::remove_if(m.begin(), m.end(), 
                 //compose1(std::bind2nd(std::less<int>(), 3), select2nd<mymap::value_type>()));
                 //boost::bind(&std::pair<int,unsigned>::second, _1) < 3);
                 //foo);
  std::cout << "Count = " 
            << std::count_if(m.begin(), m.end(), 
               compose1(std::bind2nd(std::less<int>(), 3), select2nd<mymap::value_type>()))
               // foo)
            << std::endl;
  
#ifdef __GXX_EXPRERIMENTAL_CXX0X_
  std::remove_if(m.begin(), m.end(), [](const mymap::value_type & p) { return p.second < 3; } );
#endif // __GXX_EXPRERIMENTAL_CXX0X_
  
  const mymap & cm = m;
  std::transform(cm.begin(), 
                 cm.end(), 
                 std::ostream_iterator<unsigned>(std::cout," "),
                 select2nd<mymap::value_type>());

  typedef boost::function<bool (const IntPair &, const IntPair &)> Comparator;
  Comparator c = boost::bind(&IntPair::second, _1) < boost::bind(&IntPair::second, _2);
  std::set<IntPair, Comparator> s(c);
  s.insert(IntPair(5,6));
  s.insert(IntPair(3,4));
  s.insert(IntPair(1,2));
  BOOST_FOREACH(IntPair const & p, s)
  {
    std::cout << p;
  }
}

