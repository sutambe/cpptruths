#include <iostream>

const int FIRST_TIME = 1;
template <typename T>
void func (T &)
{
 static int var;
 ++var;
 if (FIRST_TIME == var)
   std::cout << "Printed once." << std::endl;     
 else
   std::cout << "Printed more than once." << std::endl;        
}

class body;
class handle
{
 public:
   handle (body *b) : b_ (b) {}
   const body *operator -> () const { return b_; }
   body *operator -> () { return b_; }
   body *b_;
};

class body
{
  public:
     /*void func() const
     {
       std::cout << "const" << std::endl;    
     }*/

     void func()
     {
       std::cout << "non const" << std::endl;    
     }
};
int main(void)
{
     int a1[4];
     int a2[5];
     func (a1);
     func (a2);    
    
     const handle h (new body);
     h->func();
     std::string s;
     std::cin >> s;
     return 0;   
}

