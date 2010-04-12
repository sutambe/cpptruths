#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))

class A
{
  protected:
    void func () 
    { }
};

class B: private A
{
  public:
  template <class T>
  void foo ()
  {
    void (A::*ptr)() = & A::func;
    A a;
    CALL_MEMBER_FN(a,ptr)();
    A::func();
  }
  
  void bar ()
  {
    void (A::*ptr)() = & A::func;
  }
};

int main (void)
{
  B b;
  b.foo<int>();
  b.bar();
}
