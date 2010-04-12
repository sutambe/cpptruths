#include <vector>
#include <iostream>

class Test
{
  public:
    Test () {
      std::cout << "Test Constructor\n";
    }
    Test (const Test &) {
      std::cout << "Test Copy-Constructor\n";
    }
    Test & operator = (const Test &) {
      std::cout << "Test::operator = \n";
      return *this;
    }
    ~Test () {
      std::cout << "Test Destructor\n";
    }
};

int main (int argc, char *argv[])
{  
  int size = atoi(argv[1]);
  Test *array = new Test[size];
  std::cout << "============\n";
  std::vector<Test> v1(array, array + size);
  std::cout << "============\n";

  delete [] array;
  return 0;
}

