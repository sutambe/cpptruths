#include <iterator>
#include <iostream>
#include <algorithm>
#include <vector>
#include <limits>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/static_assert.hpp>

class Requester 
{
  public:
    template<typename FWIter>
    typename boost::enable_if<
        boost::is_convertible<typename std::iterator_traits<FWIter>::iterator_category, 
                              std::forward_iterator_tag>,
        void>::type

    //void
    receive(FWIter begin, FWIter end)
    {
      while(begin != end) *begin++ = 999;
    }

    template<typename OpIter>
    void receive(OpIter begin, int count, int min = std::numeric_limits<int>::max())
    {
      while(count--) *begin++ = count;
    }
};

int main(void)
{
  Requester r;
  std::vector<int> v;
  r.receive(std::back_inserter(v), 5);
  r.receive(std::ostream_iterator<int>(std::cout, "\n"), 4);
  //r.receive(std::istream_iterator<int>(std::cin), 4);
  std::cout << "\n";

  r.receive(v.begin(), v.end());
  //r.receive(std::ostream_iterator<int>(std::cout), std::ostream_iterator<int>(std::cout));
  std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, "\n"));
  std::cout << "\n";

  return 0;
}

