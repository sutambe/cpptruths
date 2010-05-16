#include <string>
#include <utility>

template <unsigned int DIM, typename T>
class Point
{
    T x[DIM];
  
  public:
    
    template<typename... U>
    Point(U... p) : x{std::move(p)...}
    { }
};

int main(void) 
{
  Point<2, int> p(1,2,3);
  
}
