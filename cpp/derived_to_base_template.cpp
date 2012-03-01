#include <cstdio>

template <class T, class U>
class Listener
{
  public:
    virtual void on_event() = 0;
    virtual ~Listener() {}
};

class MyListener : public Listener<int, double>
{
  virtual void on_event()
  {
    printf("MyListener::on_event\n");
  }
};

template <class T, class U>
void func(Listener<T, U> * listener)
{
  listener->on_event();
}
  
int main(void)
{
  func(new MyListener());
}
