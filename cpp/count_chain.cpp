#include <iostream>
#include <string>
#include <sstream>

using namespace std;

template <int SIZE>
class Base : public Base <SIZE-1>
{
  public:
    virtual Base <SIZE-1> & opt1 (const string &) = 0;
    virtual Base <SIZE-1> & opt2 (const string &) = 0;
    virtual Base <SIZE-1> & opt3 (const string &) = 0;
};

template <>
class Base <0>
{
  public:
    virtual ~Base () {}
 
  private:
    virtual Base <0> & opt1 (const string &) = 0;
    virtual Base <0> & opt2 (const string &) = 0;
    virtual Base <0> & opt3 (const string &) = 0;
};

template <int SIZE>
class Options : public Base <SIZE-1>
{
  public:
    Options ()
    : opt1_("1"), opt2_("2"), opt3_("3")
    {
    }
    
    Base <SIZE-1> & opt1 (const string & s)
    {
      opt1_ = s;
      return *this;
    }

    Base <SIZE-1> & opt2 (const string & s)
    {
      opt2_ = s;
      return *this;
    }

    Base <SIZE-1> & opt3 (const string & s)
    {
      opt3_ = s;
      return *this;
    }

    void print () const
    {  
       cout << opt1_ << endl;       
       cout << opt2_ << endl;       
       cout << opt3_ << endl;       
    }
    
  private:  
    string opt1_, opt2_, opt3_;
};

template <>
class Options<0>
{
};

int main(void)
{
  Options<2> o1;
  o1.opt1("Sumant").opt2("Uday").opt3("Tambe");
  o1.print ();
  
  string s;
  cin >> s;
  return 0;
}

/*

#include <iostream>

using namespace std;

template <int SIZE>
class Base : public Base <SIZE-1>
{
  public:
    virtual Base <SIZE-1> & add () = 0;
};

template <>
class Base <0>
{
  public:
    virtual ~Base () {}
  private:
    virtual Base <0> & add () = 0;
};

template <int SIZE>
class D : public Base <SIZE-1>
{
  public:
    D () : count_(0) {}
    Base <SIZE-1> & add ()
    {
      count_++;
      return *this;
    }
    int get () { return count_; }
  private:  
    int iov[SIZE];
    int count_;
};
          
int main(void)
{
  D<3> d1;
  d1.add().add().add();
  cout << d1.get() << endl;
  string s;
  cin >> s;
  return 0;
}

*/
