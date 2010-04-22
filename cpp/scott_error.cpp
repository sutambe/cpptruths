#ifdef  __GXX_EXPERIMENTAL_CXX0X__

#include <iostream>
#include <unordered_set>
#include <memory>
#include <type_traits>

struct Widget {
  /* explicit */ operator const char * () const { return "w"; }
};

std::ostream & operator << (std::ostream & o, const Widget & w)
{
  o << "Widget\n";
  return o;
}

int main(void)
{
  std::unordered_multiset<std::unique_ptr<Widget>> msupw;
  
  for(auto iter = msupw.cbegin(); iter != msupw.cend(); ++iter)
  {
    const auto &up = *iter;
    
    auto test = (up ? *up : "Null");
    //std::cout << up ? *up : "null" << '\n'; // Scott's original code.
    std::cout << (up ? *up : *std::unique_ptr<Widget>(new Widget()));
    std::cout << *up;
    std::cout << "Null";
    //std::common_type<Widget, const char *>::type common = "Null";
  }

}
#endif //  __GXX_EXPERIMENTAL_CXX0X__

#ifndef  __GXX_EXPERIMENTAL_CXX0X__

#include <iostream>

struct Widget { 
  operator const char * () const { return "w"; }
};

std::ostream & operator << (std::ostream & o, const Widget & w)
{
  o << "Widget\n";
  return o;
}

int main(void)
{
  Widget *w = new Widget();
  std::cout << (w ? *w : "Null") << "\n";
}

#endif 

