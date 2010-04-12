#include <iostream>

class Base1
{
  public:
      virtual int open (int) = 0;
      virtual ~Base1() {}
};

class Base2
{
  public:
      virtual int open (int) = 0;
      virtual ~Base2() {}
};
/*
class Derived : public Base1, public Base2
{
  public:
    virtual int open (int i) 
    {
      std::cout << "Inside open: " << i << "\n";
      return 0;
    }
    virtual ~Derived () {}
};
*/

class Derived // Note no inheritance
{
  class Base1_Impl;
  friend class Base1_Impl;
  class Base1_Impl: public Base1 // Note public inheritance
  {
     public:
       Base1_Impl (Derived * p) : parent_ (p) {}
       virtual int open (int) 
       {
          return parent_->base1_open ();
       }
     private:
       Derived * parent_;
  } base1_obj;   // Note member object here.

  class Base2_Impl;
  friend class Base2_Impl;
  class Base2_Impl: public Base2 // Note public inheritance
  {
     public:
       Base2_Impl (Derived * p) : parent_ (p) {}
       virtual int open (int) 
       {
          return parent_->base2_open ();
       }
     private:
       Derived * parent_;
  } base2_obj; // Note member object here

  int base1_open () 
  {
    std::cout << "base1_open\n";
    return 0;
  }
  int base2_open ()
  {
    std::cout << "base2_open\n";
    return 0;
  }

  public:
    Derived () : base1_obj (this), base2_obj(this) {}
    operator Base1 & () { return base1_obj; }
    operator Base2 & () { return base2_obj; }
};

int  base1_open (Base1 & b1)
{
    return b1.open (1);
}

int base2_open (Base2 & b2)
{
    return b2.open (2);
}

int main (void)
{
  Derived d;
  Base1 &b = *(new Derived);
  Base1 *b2 = &b;
  base1_open (d);
  base2_open (d);
  return 0;
}

