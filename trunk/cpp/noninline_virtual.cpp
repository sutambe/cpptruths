class Base
{
      public:
            virtual ~Base() throw();    
};

class Derived: public Base
{
      public:
             virtual ~Derived() throw(int);
};

int main()
{
    
}

