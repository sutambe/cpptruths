#include <cstdio>

template<class T> 
class auto_ptr 
{ 
  mutable bool owner;
  T* px;
public:
  explicit auto_ptr(T* p=0) 
       : owner(p), px(p) {}
  
  auto_ptr(const auto_ptr& r) 
       : owner(r.owner), px(r.release()) {}

  auto_ptr & operator = (const auto_ptr &r) 
  {
    if ((void*)&r != (void*)this) 
    {
      if (owner) 
        delete px;
      owner = r.owner; 
      px = r.release();
    }
    return *this;
  }

  ~auto_ptr() { if (owner) delete px; }
  T& operator*() const { return *px; }
  T* operator->() const { return px; }
  T* get()    const { return px; }
  T* release()  const { owner = false; return px; } // mutable 'ownership' changed here.
};

int main(void)
{
  auto_ptr<int> p(new int(10)), q(new int(20));
  p = q;
  printf("size = %d\n", sizeof(auto_ptr<int>));
}

