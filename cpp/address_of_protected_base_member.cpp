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
    // Calling func through a pointer or object of A is not allowed. 
    // Compiler support varies: Clang accepted all. gcc accepted the above
    // template and VC2010 and Comeau rejected it all.
    void (A::*ptr)() = & A::func; // not allowed.
    A a;
    a.func(); // not allowed.
    A::func(); // This is allowed.
  }
};

int main (void)
{
  B b;
  b.foo<int>();
  b.bar();
}
