
struct X 
{
  void foo () {}
};

int main(void)
{
  void (X::*f)() = &X::foo;
  X x;
  (x.*f)();
}

