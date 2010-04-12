#include <iostream>

using namespace std;

class X
{
  public:
    ~X()
    {
      throw 2000;
    }
};

void func (X x)
try
{
}
catch (int p)
{
  std::cout << "Caught an int exception outside func" << p << std::endl;
}


int main (int argc, char *argv[])
  try
{
  try {
    X x;
    try {
      int i = 2;
      func (x);
      //throw i;  
    }
    catch (int p)
    {
      cout << "int exception caught in main nested try." << p << endl;
    }
  }
  catch (int p)
  {
      cout << "int exception caught in main outer try." << p << endl;
  }
  return 0;
}
catch (...)
{
  int i = argc;
  cout << "An exception was thrown " << i << endl;
}

