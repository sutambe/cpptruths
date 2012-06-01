/*
 * Compile it with --no-elide-constructors on gcc
 *
 * */

#include <iostream>
#include <vector>

#ifndef BOOST_MOVE

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
class MovableBase
{
  public:
    T * data_;
};

template <class T>
class MovableResource 
  //: public MovableBase<T>
{
  private:
    T * resource_;

  public:
    typedef detail::proxy<T> proxy;

    explicit MovableResource (T * ptr = 0) throw ()
      : resource_ (ptr)
    {}    

  private:  
    // As per language rules, the following guys IS a copy-ctor.
    // Check Section 12.8.2. So there is no question of providing 
    // one takes a const parameter.
    MovableResource (MovableResource &m) throw ();
    //MovableResource (const MovableResource &m) throw ();
    MovableResource & operator = (MovableResource &m) throw ();
    //MovableResource & operator = (const MovableResource &m) throw ();
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
    /*
    MovableResource (MovableBase<T> base) throw () // The proxy move constructor
      : resource_(base.data_)
    {
      std::cout << "base to MovableResource constructor" << std::endl;
    }
    operator MovableBase<T> () throw ()
    {
      MovableBase<T> base;
      base.data_ = this->resource_;
      this->resource_ = 0;
      std::cout << "MovableResource to base conversion operator" << std::endl;
      return base;
    }
    */
    ~MovableResource () throw ()
    {
      delete resource_;
      std::cout << "MovableResource destroyed" << std::endl;
    }     
    
    friend MovableResource move(MovableResource & mr) // Convert to a reference to rvalue
    {
      return MovableResource(detail::proxy<T>(mr));
    }

    /* 
    // This straight-forward conversion does not work.
    detail::proxy<T> move(MovableResource & mr)
    {
      return detail::proxy<T> (mr);
    }
    */
};

#endif // BOOST_MOVE

#ifdef BOOST_MOVE

#include <boost/move/move.hpp>
#include <boost/container/vector.hpp>

template <class T>
class MovableResource
{
  private:
    T * resource_;
    BOOST_MOVABLE_BUT_NOT_COPYABLE(MovableResource)

  public:

    explicit MovableResource (T * ptr = 0) throw ()
      : resource_ (ptr)
    {}    
    MovableResource(BOOST_RV_REF(MovableResource) mr)
      : resource_(mr.resource_)
    {
      mr.resource_ = 0;
    }
    MovableResource & operator = (BOOST_RV_REF(MovableResource) mr)
    {
      delete resource_;
      resource_ = mr.resource_;
      mr.resource_ = 0;
      return *this;
    }

  public:
    void swap (MovableResource &m) throw ()
    {
      std::swap (this->resource_, m.resource_);
    }
    ~MovableResource () throw ()
    {
      delete resource_;
      std::cout << "MovableResource destroyed" << std::endl;
    }     
};

#endif

MovableResource<int> source() 
{ 
  std::cout << "source\n";
  //MovableResource<int> mr (new int(999)); 
#ifdef BOOST_MOVE
  return boost::move(mr);
#else
  //return move(mr);
  return MovableResource<int>(MovableResource<int> (new int(999))); 
  //return MovableResource<int> (new int(999)); 
#endif
}

void sink(MovableResource<int> mr) 
{ 
  std::cout << "sink\n";
}

int main ()
{
#ifdef BOOST_MOVE  
  source();
  MovableResource<int> mr2(source());
  MovableResource<int> mr3; //(mr);
  mr3 = boost::move(mr2);
  sink(boost::move(mr3));
  boost::container::vector<MovableResource<int> > v;
  v.push_back(boost::move(mr2));
  v.push_back(mr3);
#else
  //source();
  //MovableResource<int> mr1(move<int>(source()));
  //MovableResource<int> mr2(source());
  MovableResource<int> mr3; //(mr);
  mr3 = source();
  sink(move(mr3));
  //mr2 = mr;
#endif  
  std::cout << "All temporaries should be gone by now" << std::endl;
}


