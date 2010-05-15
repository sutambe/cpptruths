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
      operator Container () {
        Container c;
        for(size_t i = 0;i < count; ++i)
          c.insert(c.end(), rand());
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

    // std::auto_ptr<int> ap = MakeSmartPtr(); // does not compile
    // boost::scoped_ptr<int> sc_ptr = MakeSmartPtr(); // does not compile
    boost::shared_ptr<int> sh_ptr = MakeSmartPtr();

    std::set<int> random_s = getRandomN(10);
    std::vector<int> random_v = getRandomN(10);
    std::list<int> random_l = getRandomN(10);
    std::list<int> random_l2 = getRand<std::list<int> >(10);

    std::cout << il << "\n" <<  fl << "\n" << b << std::endl;
}

