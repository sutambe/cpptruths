#include <iostream>

class Base
{
  public:
    void read() const
    {
      read_impl();
    }
    virtual ~Base() {}
  private:
    virtual void read_impl () const = 0;
};

class D : public Base
{
  public:
    virtual void read_impl () const
    {
      std::cout << "D::read_impl\n";
    }
};

int main(void)
{
  D d;
  Base const & b = d;
  b.read();

  return 0;
}

