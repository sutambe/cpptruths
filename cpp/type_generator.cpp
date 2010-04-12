#include <ext/malloc_allocator.h>
#include <map>
#include <string>
#include <functional>
#include <algorithm>
#include <iostream>
#include <iterator>

using namespace std;

template <class Value>
struct Directory
{
  typedef map <string, Value, less<string>, __gnu_cxx::malloc_allocator<std::string> > type;
  typedef _Select1st<typename type::value_type> KeySelector;
  typedef _Select2nd<typename type::value_type> ValueSelector;
};

int main(void)
{
  Directory<int>::type age;
  age["Sumant"] = 500;
  age["Archana"] = 600;

  transform(age.begin(), age.end(), 
            ostream_iterator<int>(cout, "\n"), 
            Directory<int>::ValueSelector());

  transform(age.begin(), age.end(), 
            ostream_iterator<string>(cout, "\n"), 
            Directory<int>::KeySelector());
  
  transform(age.begin(), age.end(), 
            ostream_iterator<string>(cout, "\n"), 
            _Select1st<std::map<std::string, int>::value_type> ());
  

  return 0;
}
