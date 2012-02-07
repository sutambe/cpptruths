class Foo; 

Foo func(Foo f); // return or pass-by-value does no need full class definition.

class Foo
{
};

int main(void)
{
  func(Foo());
  return 0;
}

Foo func(Foo f)
{
  return f;
}

