#include <iostream>
#include <string>
#include <cstdlib>

class Test
{
    std::string value;
    int data;
  public:
    Test (std::string v) 
      : value(v), data(-999) 
    { std::cout << value << ":" << data << std::endl; }
    
    Test (int i) 
      : value("default"), data(i) 
    { std::cout << value << ":" << data << std::endl; }

    Test (const Test & t)
      : value (t.value), data (t.data)
    { std::cout << "Copy-ctor " << value << ":" << data << std::endl; }
    
    ~Test () 
    { std::cout << "Destroy " << value << ":" << data << std::endl; }
};

// Showing Return Value Optmization
// All return statements construct an object. 
Test RVO()
{
  if (random() % 2)
    return Test ("t1");
  else
    return Test (500);
}


// Showing Named Return Value Optmization
// Returning the SAME named object. 
Test NRVO()
{
  Test t2("t2");
  if (random() % 2)
    return t2;
  else
    return t2;
}

int main (void)
{
  srandom(static_cast<int>(time(0)));
  
  std::cout << "RVO" << std::endl;

  // Left hand side must be a declaration.
  Test r = RVO();
  std::cout << "NRVO" << std::endl;
  Test n = NRVO();

  return 0;
}


