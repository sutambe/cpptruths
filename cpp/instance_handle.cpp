#include <iostream>

namespace dds { namespace core {

struct null_type {};

/**
 * This class provides the basic behavior for Value types. 
 */
template <typename D>
class Value {
protected:
    Value() { }
    Value(const Value& p) : d_(p.d_) { }

public:
    template <typename ARG>
    Value(const ARG& arg) : d_(arg) { }

    template <typename ARG1, typename ARG2>
    Value(const ARG1& arg1, const ARG2& arg2) : d_(arg1, arg2) { }
    
    template <typename ARG1, typename ARG2, typename ARG3>
    Value(const ARG1& arg1, const ARG2& arg2, const ARG3& arg3)
    : d_(arg1, arg2, arg3) { }
    
public:
    ~Value() { }

public:
    Value& operator=(const Value& other) {
        if (this != &other) {
            d_ = other.d_;
        }
        return *this;
    }

    bool operator==(const Value& other) {
    	return (d_ == other.d_);
    }

    bool operator !=(const Value& other) {
    	return !(d_ == other.d_);
    }

public:
    const D* operator->() const { return d_; }
    D* operator->() { return &d_; }

    const D& delegate() const { return d_; }
    D& delegate() { return d_;}

    operator const D &() const { return d_; }
    operator D &() { return d_; }

protected:
    D d_;
};

} } /* dds / core */



namespace tdds { namespace core {

template <typename DELEGATE>
class InstanceHandle : public dds::core::Value<DELEGATE> {
public:
    InstanceHandle() { }

    InstanceHandle(const DELEGATE & d) 
      : dds::core::Value<DELEGATE>(d) {}

    InstanceHandle(const dds::core::null_type& src)
    : dds::core::Value<DELEGATE>(src) { }

    InstanceHandle(const InstanceHandle& src)
        : dds::core::Value<DELEGATE>(src.delegate()) { }

    ~InstanceHandle() { }

public:
    static const InstanceHandle nil() {
        return InstanceHandle();
    }

    bool operator==(const InstanceHandle& other) const {
    	return this->delegate().operator==(other);
    }

    InstanceHandle& operator=(const dds::core::null_type& src) {
    	return this->delegate().operator=(src);
    }

    bool operator==(const dds::core::null_type& other) const {
    	return this->is_nil();
    }

    bool is_nil() const {
    	return this->delegate().is_nil();
    }
private:
    typedef void (InstanceHandle::*safe_bool_type)() const;
    void support_safe_bool_idiom() const {}
public:
    operator safe_bool_type() const {
      return this->is_nil()?
        0: &InstanceHandle::support_safe_bool_idiom;
    }

};

template <class D>
bool operator == (dds::core::null_type, InstanceHandle<D> const &ih)
{
  return ih.is_nil();
}

template <class D>
bool operator != (dds::core::null_type, InstanceHandle<D> const &ih)
{
  return !ih.is_nil();
}

} } // tdds / core  

namespace dds 
{
  static dds::core::null_type null;
}

struct Foo {
  void *ptr;
  Foo(void * p = 0) : ptr(p) {}
  bool is_nil () const { return ptr==0; }
  Foo & operator = (dds::core::null_type & null) { ptr = 0; return *this; }
};

int main(void)
{
  int i = 10;
  tdds::core::InstanceHandle<Foo> ih_foo((Foo(&i)));

  if(ih_foo == dds::null)
    std::cout << "if(ih_foo == dds::null)\n";

  if(dds::null == ih_foo)
    std::cout << "if(dds::null == ih_foo)\n";
  
  if(ih_foo)
    std::cout << "if(ih_foo)\n";

  Foo f;
  f = ih_foo;
}

