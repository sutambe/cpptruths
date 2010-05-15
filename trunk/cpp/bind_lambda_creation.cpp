#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <functional>
#include <boost/bind.hpp>
//#include <boost/lambda/lambda.hpp>
//#include <boost/lambda/bind.hpp>

using namespace std;
using namespace boost;

template <class InputIterator, class OutputIterator, class Predicate>
OutputIterator copy_if(InputIterator first, InputIterator last,
                       OutputIterator result, Predicate pred)
{
  typedef typename Predicate::result_type r;
  while(first!=last)
  {
    if(pred(*first))
      *result++ = *first;
    ++first;
  }
  return result;
}

struct A
{
  A (const string & name) : m_name(name) {}
  bool operator == (const A & a) const {
    return m_name == a.m_name;
  }

  string &        name ()         { return m_name; }
  const string &  name () const   { return m_name; }

  string m_name;
};

std::ostream & operator << (std::ostream & o, const A & a)
{
  return o << a.name();
}

int main () 
{
  vector<A> av;
  av.push_back (A ("some name"));
  av.push_back (A ("some other name"));

  copy_if(av.begin(), av.end(), 
          std::ostream_iterator<A>(std::cout, "\n"),
          //bind<const string &>(&A::name, _1) == "some name"); // Works
          bind<const string &>(&A::m_name, _1) == "some name"); // Works
          //bind(std::equal_to<string>(), bind<const string &>(&A::m_name, _1), "some name")); // Works
          //bind(std::equal_to<A>(), _1, A("some name"))); // Works
          //std::bind2nd(std::equal_to<A>(), A("some name"))); // Works

  return 0;
}
