#include <cstdio>

class Base {
private:
  virtual void func(int x) = 0;
  friend class Attorney;

public:
  ~Base() {}
};

class Derived : public Base {
private:
  virtual void func(int x)
  {
    printf("Derived::func\n");
  }

public:
  virtual ~Derived() {}
};

class Attorney {
private:
  static void callFunc(Base & b, int x) {
    return b.func(x);
  }
  friend int main (void);
};

int main(void)
{
  Derived d;
  Attorney::callFunc(d, 10);
}
