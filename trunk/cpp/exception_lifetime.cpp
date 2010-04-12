#include <iostream>

using namespace std;

struct Exception
{
   Exception (int i) : i_ (i) {
      cerr << "Constructing Exception " << i_ << "\n";
   }

   Exception (const Exception &e) : i_(e.i_ * 2) {
      cerr << "Copy constructing Exception "  << i_ << "\n";
   }

   ~Exception () {
      cerr << "Destroying Exception " << i_ << "\n";
   } 
   int i_;

};

struct A
{
  A () {}
  ~A () 
   {
     cerr << "A::~A()\n"; 
     if (! std::uncaught_exception ()) 
         throw Exception (10); 
   }
};

void func (void)
{
  A a;
  //throw Exception (20);
  A a2;
}

int main(void)
{
  Exception *eptr;
  try 
  {
    func ();
  }
  catch (Exception &e) 
  {
    eptr = &e;
    cerr << e.i_ << endl;
  }
  cerr << "After catch " << eptr->i_ << "\n";
  

  return 0;
}


