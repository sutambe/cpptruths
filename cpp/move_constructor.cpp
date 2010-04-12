#include <iostream>

static int count = 0;

template <class T>
class MoveResource
{
  private:
    struct proxy
    {
      bool def;
      T * resource_;
      proxy() : def(true) { }
      proxy(const proxy & p) : def(false), resource_(p.resource_) 
      { std::cout << "proxy copy-constructor" << std::endl; count++; }
      ~proxy() 
      { 
        if (def) 
          std::cout << "default constructed proxy destroyed: " << count << std::endl; 
        else 
          std::cout << "copy constructed proxy destroyed: " << count << std::endl; 
      }
    };
    T * resource_;
  public:
    MoveResource () throw ()
      : resource_ (0)
    {}    
    
    MoveResource (MoveResource &m) throw () // Move constructor
      : resource_ (m.resource_)
    {
      m.resource_ = 0;
    }
    MoveResource (proxy p) throw () // The proxy move constructor
      : resource_(p.resource_)
    {
      std::cout << "proxy to MoveResource constructor" << std::endl;
    }
    MoveResource & operator = (proxy p) throw ()
    {
      MoveResource temp (p);
      temp.swap (*this); // copy and swap idiom
      return *this;
    }
    MoveResource & operator = (MoveResource &m) throw ()
    {
      MoveResource temp (m);
      temp.swap (*this); // copy and swap idiom
      return *this;
    }
    void swap (MoveResource &m) throw ()
    {
      std::swap (this->resource_, m.resource_);
    }
    operator proxy () throw ()
    {
      proxy p;
      p.resource_ = this->resource_;
      this->resource_ = 0;
      std::cout << "MoveResource to proxy conversion operator" << std::endl;
      return p;
    }
    ~MoveResource () throw ()
    {
      std::cout << "MoveResource destroyed" << std::endl;
    }    
    
};

MoveResource <int> f () { return MoveResource <int>(); }
int main ()
{
  MoveResource <int> m (f());
  std::cout << "All temporaries should be gone by now" << std::endl;
}

