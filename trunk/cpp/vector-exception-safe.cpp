#include <iostream>
#include <vector>

class Box
{
    static int count;
  public:
    Box() 
    { }
    Box(const Box & b2) 
    { 
      if (count == 5)
      {
        throw count;
      }
      count++;
      std::cout << "Box (copy-constructor) created: " << count << std::endl;
    }
    ~Box()
    {
      std::cout << "Box destroyed: " << count << std::endl;
      --count;
    }
};
int Box::count = 0;

int main (void)
{
  try {
    Box b;
    std::vector<Box> v(10, b);
  }
  catch(...)
  {
    std::cout << "Exception caught.\n";
  }
  return 0;
}

