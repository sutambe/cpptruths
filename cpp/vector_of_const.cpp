#include <iostream>
#include <vector>
#include <list>

int main (void)
{
  const int SIZE = 5;
  int array [SIZE] = { 10, 20, 30, 40, 50 };
  
  std::vector<int> v1;
  v1.push_back(array[0]);
  std::list<const int> l1;

}

