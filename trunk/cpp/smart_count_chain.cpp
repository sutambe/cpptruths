#include <cstdio>

template <int SIZE>
class X;

template <typename T>
class dispatch
{
    T &ref;
    static int count_;
  public:
    dispatch(T& x): ref(x) { ++count_; }
    ~dispatch() { if (0 == --count_) ref.real_send(); }
    T* operator->(){ return &ref; }
};

template <typename T, int SIZE>
class Base : public Base <T, SIZE-1>
{
  public:
    virtual Base <T, SIZE-1> & send (int i) = 0;
    virtual dispatch<T> operator -> () = 0;
};

template <typename T>
class Base <T, 0>
{
  public:
    virtual ~Base () {}
    virtual Base & send (int i) = 0;
  
  private:  
    virtual dispatch<T> operator -> () = 0;
};

template <int SIZE>
class X : public Base <X <SIZE>, SIZE-1>
{
  public:
    X<SIZE> () { }
    Base <X, SIZE-1> & send (int i) { printf("send %d\n", i); return *this;}
    void add(int i) { printf("add %d\n", i); }
    void real_send() { printf("real_send\n"); }
    dispatch < X<SIZE> > operator->() { return *this; }
};

template <typename T>
int dispatch<T>::count_ = 0;

int main(void)
{
  X<2> x;
  //Base <X<3>, 0> &b = x;
  //b->send(3);
  printf("++++++\n");
  x->send(1)
   ->send(2)
   ->send(3) 
   ->send(4);
  printf("------\n");
  x.send(2);
  
  return 0;
}    


