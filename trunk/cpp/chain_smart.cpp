#include <iostream>
#include <cstdio>

using namespace std;

class X;

class dispatch
{
  static X *xref;
public:
  dispatch(X& x);
  ~dispatch();
  X* operator->();
};

X *dispatch::xref = 0;

class X
{
public:
  X () { }
  dispatch send(int i) { add(i); return *this; }
  void add(int i) { printf("add %d\n",i); }
  void real_send() { printf("real_send\n"); }
  dispatch operator->() { return *this; }
};

dispatch::dispatch(X& x) 
{ 
  if (!xref) xref = &x;
}
dispatch::~dispatch() 
{ 
  if (xref) 
  { 
    xref->real_send(); 
    xref = 0; 
  } 
}
X* dispatch::operator->() { return xref; }

int main(void)
{
  X x;
  printf("++++++\n");
  x->send(1)->send(2);
  printf("------\n");
  x.send(10);
  return 0;
}     

