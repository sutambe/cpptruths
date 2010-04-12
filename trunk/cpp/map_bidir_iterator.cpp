#include <map>
#include <iostream>
#include <algorithm>
#include <string>
#include <list>
#include <iterator>

template <class BiDirIter>
class StdMapIteratorAdaptor 
  : public
    std::iterator <std::bidirectional_iterator_tag,
                   typename BiDirIter::value_type::second_type>
{
    BiDirIter iter_;
  public:

    explicit StdMapIteratorAdaptor(BiDirIter const & iter = BiDirIter())
      : iter_(iter) {}

    bool operator == (StdMapIteratorAdaptor const & rhs) const {
      return (iter_ == rhs.iter_);
    }

    bool operator != (StdMapIteratorAdaptor const & rhs) const {
      return !(*this == rhs);
    }
    
    typename BiDirIter::value_type::second_type const & operator * () {
      return iter_->second;
    }

    typename BiDirIter::value_type::second_type const & operator * () const {
      return iter_->second;
    }

    typename BiDirIter::value_type::second_type const * operator -> () const {
      return &(iter_->second);
    }

    StdMapIteratorAdaptor & operator ++ () {
      ++iter_;
      return *this;
    }

    const StdMapIteratorAdaptor operator ++ (int) {
      StdMapIteratorAdaptor temp (iter_);
      ++iter_;
      return temp;
    }

    StdMapIteratorAdaptor & operator -- () {
      --iter_;
      return *this;
    }

    const StdMapIteratorAdaptor operator -- (int) {
      StdMapIteratorAdaptor temp (iter_);
      --iter_;
      return temp;
    }
};

template <class BiDirIter>
StdMapIteratorAdaptor <BiDirIter> 
std_map_iterator_adaptor (BiDirIter const & iter)
{
  return StdMapIteratorAdaptor<BiDirIter> (iter);
}

int main(void)
{
  typedef std::map <std::string, int> StrIntMap;
  StrIntMap months;
  
  months["january"] = 31;
  months["february"] = 28;
  months["march"] = 31;
  months["april"] = 30;
  months["may"] = 31;
  months["june"] = 30;
  months["july"] = 31;
  months["august"] = 31;
  months["september"] = 30;
  months["october"] = 31;
  months["november"] = 30;
  months["december"] = 31;
 
  StrIntMap const & m = months;

  StdMapIteratorAdaptor <StrIntMap::const_iterator> begin (m.begin());
  StdMapIteratorAdaptor <StrIntMap::const_iterator> end (m.end());
  std::copy(begin, end, std::ostream_iterator <int> (std::cout, " "));
  std::cout << std::endl;
  
  std::list<int> l(std_map_iterator_adaptor(m.begin()), 
                   std_map_iterator_adaptor(m.end()));

  std::copy (l.begin(), l.end(), std::ostream_iterator <int> (std::cout, " "));
  std::cout << std::endl;
  std::copy (std_map_iterator_adaptor(months.begin()), 
             std_map_iterator_adaptor(months.end()), 
             std::ostream_iterator <int> (std::cout, " "));
  std::min_element (std_map_iterator_adaptor(months.begin()),
                    std_map_iterator_adaptor(months.end()));
    
  return 0;
}

