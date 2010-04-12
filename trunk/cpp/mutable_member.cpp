#include <iostream>
#include <vector>

class X
{
    mutable std::vector<int> v;
  public:
    typedef std::vector<int>::iterator iterator;

    iterator begin() { return v.begin(); }
    iterator begin() const { return v.begin(); }
    
    iterator end() { return v.end(); }
    iterator end() const { return v.end(); }
    
};

int main(void)
{
  const X x;
  std::vector<int>::iterator iter = x.begin();

}
