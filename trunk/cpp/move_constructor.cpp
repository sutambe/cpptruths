#include <iostream>

static int count = 0;

namespace detail {

template <class T>  
struct proxy
{
  bool def;
  T *resource_;
  
  proxy() : def(true) { }
  proxy(const proxy & p) 
    : def(false), resource_(p.resource_) 
  { 
    std::cout << "proxy copy-constructor" << std::endl; count++; 
  }
  ~proxy() 
  { 
    if (def) 
      std::cout << "default constructed proxy destroyed: " << count << std::endl; 
    else 
      std::cout << "copy constructed proxy destroyed: " << count << std::endl; 
  }
};

} // detail

template <class T>
class MovableResource
{
  private:
    T * resource_;

  public:
    MovableResource (T * ptr = 0) throw ()
      : resource_ (ptr)
    {}    
  private:  
    MovableResource (MovableResource &m) throw ();
    //MovableResource (const MovableResource &m) throw ();
    MovableResource & operator = (MovableResource &m) throw ();
    MovableResource & operator = (const MovableResource &m) throw ();
/*  
  public:
    MovableResource (MovableResource &m) throw () // Move constructor
      : resource_ (m.resource_)
    {
      m.resource_ = 0;
    }
    MovableResource & operator = (MovableResource &m) throw ()
    {
      MovableResource temp (m);
      temp.swap (*this); // copy and swap idiom
      return *this;
    }
*/
  public:
    MovableResource (detail::proxy<T> p) throw () // The proxy move constructor
      : resource_(p.resource_)
    {
      std::cout << "proxy to MovableResource constructor" << std::endl;
    }
    MovableResource & operator = (detail::proxy<T> p) throw ()
    {
      MovableResource temp (p);
      temp.swap (*this); // copy and swap idiom
      return *this;
    }
    void swap (MovableResource &m) throw ()
    {
      std::swap (this->resource_, m.resource_);
    }
    operator detail::proxy<T> () throw ()
    {
      detail::proxy<T> p;
      p.resource_ = this->resource_;
      this->resource_ = 0;
      std::cout << "MovableResource to proxy conversion operator" << std::endl;
      return p;
    }
    ~MovableResource () throw ()
    {
      delete resource_;
      std::cout << "MovableResource destroyed" << std::endl;
    }     
};

template <class T>
detail::proxy<T> move(MovableResource<T> & mr)
{
  return detail::proxy<T> (mr);
}

MovableResource<int> f () 
{ 
  MovableResource<int> mr (new int(999)); 
  return move(mr);
}

int main ()
{
  MovableResource<int> mr(f());
  MovableResource<int> mr2; //(mr);
  mr2 = move(mr);
  mr2 = mr;
  
  std::cout << "All temporaries should be gone by now" << std::endl;
}

