struct X
{
  X () {}
  explicit X (const X &) {}
};
/*
X foo ()
{
  X x;
  return x;
}
*/
void bar (X x)
{
}
int main(void)
{
  X y;
  //y = foo();
  bar (y);
  X z(y);
}
