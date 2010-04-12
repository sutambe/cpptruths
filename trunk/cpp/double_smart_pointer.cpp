#include <cstdio>

      class X;
      
      class dispatch
      {
        X& xref;
      public:
        dispatch(X& x);
        ~dispatch();
        X* operator->();
      };
      
      class X
      {
        char name;
      public:
        X(char c) : name(c) { }
        void doSomething() { printf("doSomething with %c\n", int(name)); }
        void lock() { printf("lock %c\n", name); }
        void unlock() { printf("unlock %c\n", name); }
        dispatch operator->() { return *this; }
      };
      
      dispatch::dispatch(X& x) : xref(x) { xref.lock(); }
      dispatch::~dispatch() { xref.unlock(); }
      X* dispatch::operator->() { return &xref; }
      
      int main(void)
      {
        X x('x');
        printf("++++++\n");
        x->doSomething();
        printf("------\n");
        x.doSomething();
        return 0;
      } 

