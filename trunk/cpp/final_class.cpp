class MakeFinal
{
    ~MakeFinal() {}
    friend class last;
};

class last : virtual MakeFinal
{ 
  public:
    static void foo() { }
};

class test : public last
{ 
  public:
    static void bar() {
      last::foo();
    }
};

int main(void)
{
  test::bar();
  //test t;
}
