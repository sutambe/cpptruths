template <typename T>
class Foo
{
public:
  virtual void visit () {}

  int i_;
};

template <typename T>
class Bar : public Foo<T>
{
public:
  virtual void visit ()
  {
    // Fails:
    i_ = 0;

    // Works:
    Foo<T>::i_ = 0;
  }
};

class Foo2
{
public:
  virtual void visit () {}

  int i_;
};

class Bar2 : public Foo2
{
public:
  virtual void visit ()
  {
    // Fails:
    i_ = 0;

    // Works:
    Foo2::i_ = 0;
  }
};
int
main (int argc, char *argv[])
{
  Bar2 b2;
  return 0;
}
