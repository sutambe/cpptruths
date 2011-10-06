#include <cstdio>
#include <utility>
#include <vector>
#include <memory>

#ifdef MOVE

template <class T>
class auto_ptr
{
  private:
    struct proxy
    {
      T * resource_;
    };
    T * resource_;
  public:
    auto_ptr (T * r) : resource_(r) { }  
    auto_ptr (auto_ptr &m) throw () // Move constructor (note non-const parameter)
      : resource_ (m.resource_)
    {
      m.resource_ = 0;
    }
    auto_ptr (proxy p) throw () // The proxy move constructor
      : resource_(p.resource_)
    {
    }
    auto_ptr & operator = (auto_ptr &m) throw ()
    {
      // copy and swap idiom
      auto_ptr temp (m);
      temp.swap(*this);
      return *this;
    }
    auto_ptr & operator = (proxy p) throw ()
    {
      // copy and swap idiom
      auto_ptr temp (p);
      temp.swap(*this);
      return *this;
    }
    void swap (auto_ptr &m) throw ()
    {
      std::swap (this->resource_, m.resource_);
    }
    operator proxy () throw () // A helper conversion function. Note that it is non-const
    {
      proxy p;
      p.resource_ = this->resource_;
      this->resource_ = 0;
      return p;
    }
};

#else

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

#endif

int main(void)
{
  auto_ptr<int> p(new int(10)), q(new int(20));
  p = q;
  printf("size = %d\n", sizeof(auto_ptr<int>));
  std::vector<auto_ptr<int> > v_ptr(10);
}

