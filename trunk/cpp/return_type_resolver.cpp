#include <memory>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <list>
#include <set>
#include <algorithm>

template<typename T>
T ft() { return T(); }
/*  
template<>
int ft<int>() { return 1; }
   
template<>
float ft<float>() { return 0.5; }
  */  
struct Resolver
{
    template<typename T>
    operator T() { return ft<T>(); }
};

struct MakeSmartPtr
{
  template <class SmartPtr>
  operator SmartPtr () {
    return SmartPtr(new typename SmartPtr::element_type());
  }
};

class getRandomN {
    size_t count;
  
  public:
    getRandomN(int n = 5) : count(n) {}

    template <class Container>
    operator Container () 
    {
        Container c;
        for(size_t i = 0;i < count; ++i)
          c.insert(c.end(), rand());
          //c.push_back(rand());
        return c;
    }
};

template <class Container>
Container getRand(size_t n) {
  Container c;
  for(size_t i = 0;i < n; ++i)
    c.insert(c.end(), rand());
  return c;
}

int main()
{
    Resolver resolver; 
    int il = resolver;
    float fl = Resolver();
    bool b;
    b = resolver;

    std::auto_ptr<int> ap = MakeSmartPtr(); 
    // This idiom does not work with std::auto_ptr because the compiler must
    // attempt two type conversions to accomplish it but C++ allows exactly
    // one type conversion from rvalue to lvalue. MakeSmartPtr uses the first
    // slot for automatic type conversion so there is no slot left for
    // std::auto_ptr's magic through auto_ptr_ref for returning by value
    // from a function.
    
    // boost::scoped_ptr<int> sc_ptr = MakeSmartPtr(); 
    // Does not compile because scoped_ptr can't be returned.
    
    boost::shared_ptr<int> sh_ptr = MakeSmartPtr(); // works!

    std::set<int> random_s = getRandomN(10);
    std::vector<int> random_v = getRandomN(10);
    std::list<int> random_l = getRandomN(10);
    std::list<int> random_l2 = getRand<std::list<int> >(10);

    std::cout << il << "\n" <<  fl << "\n" << b << std::endl;
}

