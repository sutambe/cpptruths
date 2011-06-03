#include <memory>
#include <type_traits>

class Base
{
public:
};

class Derived : public virtual Base
{
public:
};

template <typename T>
  class BaseRef_T
  {
  public:
    BaseRef_T (std::shared_ptr <T> t) : impl_ (t) {};
    BaseRef_T (T* t) : impl_ (t) {};
    BaseRef_T (void) : impl_ () {};
    template <class U>
    BaseRef_T (const BaseRef_T<U> & baseu): impl_(baseu.impl_) {
      static_assert(std::is_base_of<T, U>::value, "T is not base of U");
    }
    template <class U>
    BaseRef_T & operator = (const BaseRef_T<U> & baseu) {
      static_assert(std::is_base_of<T, U>::value, "T is not base of U");
      this->impl_=baseu.impl_;
      return *this;
    }
    typedef T element_type;
    inline T* operator-> () { return impl_.get ();}
    inline T* get() const { return impl_.get ();}
    explicit operator bool() const // never throws
      { return impl_ == 0 ? false : true; }
    void
    operator=(const std::nullptr_t& t) // never throws
    {
     impl_ = t;
    }

//  protected:
    typedef std::shared_ptr <T> shared_ptr_type;
    shared_ptr_type impl_;
  };
typedef BaseRef_T <Base> BaseRef;


template <typename T>
  class Derived_objref
    : public virtual BaseRef_T <T>
  {
  public:
    Derived_objref () : BaseRef_T<T> () {}
    Derived_objref (T *s) : BaseRef_T<T> (s) {}
    Derived_objref (const BaseRef_T <T>& o) : BaseRef_T <T> (o) {}
    void operator=(const std::nullptr_t& t) { this->impl_ = t; }
    typedef int my_index_type; // Needed as additional typedef for Derived
    struct Foo {int a;};
  };
typedef Derived_objref <Derived> DerivedRef;

int main (int,char*[])
{
  std::shared_ptr<Base> bs (new Base);
  std::shared_ptr<Derived> ds (new Derived);
  std::shared_ptr<Base> cs = ds;
  BaseRef b (new Base);
  DerivedRef d(new Derived);
  BaseRef c = d; // <== how to get this working?
  c = d;
  return 0;
}
