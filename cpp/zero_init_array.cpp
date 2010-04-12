#include<iostream>

using namespace std;

template <class T>
void foo(void)
{
  T *x = new T[5]; // Memory UnInitialised
  T *y = new T[5]();
  T test;

  for(int i = 0; i < 5; i++)
  {
    cout<<"x["<<i<<"]="<<x[0]<<endl;
    cout<<"y["<<i<<"]="<<y[0]<<endl;
  }
  cout << test << endl;

  delete[] x;
  delete[] y;
}

struct X
{
  int i;
  X() : i(5) {}
};

ostream & operator << (ostream & o, const X & x)
{
  o << x.i;
  return o;
}

int main(void)
{
  foo<int>(); 
  foo<X>(); 

  return 0;
}
