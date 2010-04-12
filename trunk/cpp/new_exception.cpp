#include <iostream>

class Foo
{
  public:
         Foo (int i) 
         {
            std::cout << "FOO: " << i << std::endl;
         }
                   
};

int func (int x)
{
       if (!x)
          throw x;
       return x;
}
int main (void)
{
    try {
        Foo *f = new Foo (func(0));
    }
    catch (int i)
    {
          std::cout << i << std::endl;      
    }
    getchar();
    return 0;
}

