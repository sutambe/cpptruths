#include <vector>
#include <iostream>
#include <list>
#include <cstdlib>

void * operator new (size_t size)
{
  std::cout << "size = " << size << std::endl;
  return malloc(size);
}

void operator delete (void *ptr)
{
  free(ptr);
}

struct Test 
{
  static size_t count;

  Test() {}
  
  Test (const Test &) {
    //std::cout << "Test copied\n";
    ++count;
  }
  
  ~Test() {
    //std::cout << "Test destroyed\n";
  }
};

size_t Test::count = 0;

int main(void)
{
  const int N = 10000;
  std::list<Test> l(N);
  Test::count = 0;

  std::vector<Test> v(1);
  v.insert(v.end(), l.begin(), l.end()); // Just one allocation here
  std::cout << Test::count << std::endl;
  
  return 0;
}
