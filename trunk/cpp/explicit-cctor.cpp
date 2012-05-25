struct X
{
  X () {}
  explicit X (const X &) {}
};

X foo ();
/*
X foo()
{
  X x;
  return x; // named return does not work.
}
*/
X bar();
/*
X bar()
{
  return X(); // rvalue return does not work.
}
*/
void func(X x)
{
}
int main(void)
{
  X x1;
  //x1 = foo();
  //func(x1);
  X x2(X(X(x1))); // this wroks.
}
