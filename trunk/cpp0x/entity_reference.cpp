#include <memory>
#include <vector>
#include <iostream>
#include <cstddef>
#include <functional>
#include <stdexcept>
#include <algorithm>

namespace dds {

template <class T>
class Listener;

template <class D>
class TReference
{
  std::shared_ptr<D> ptr_;

public:
  TReference()
    : ptr_(nullptr) {}

  template <class X>
  //TReference(X && x) : ptr_(std::make_shared<D>(std::forward<X>(x))) {}
  TReference(X && x) : ptr_(new D(x), std::mem_fun(&D::try_delete)) {}

  D * delegate() const { return ptr_.get(); }
  void operator = (std::nullptr_t) { ptr_.reset(); }
};

template <class D>
class TEntity : public TReference<D>
{
public:
  TEntity() {} 

  template <class X>
  TEntity(X && x) 
    : TReference<D>(std::forward<X>(x))
  {}

  void retain() { TReference<D>::delegate()->retain(); }
  void close() { TReference<D>::delegate()->close(); }
};

template <class D>
class TSubscriber : public TEntity<D>
{
public:
  TSubscriber() {}
  TSubscriber(int i) : TEntity<D>(i) {}
};

template <class T, template <class> class D>
class TDataReader : public TEntity<D<T>>
{
public:

  TDataReader() {} 

  template <class SD>
  TDataReader(TSubscriber<SD> & sub)
    : TEntity<D<T>>(sub.delegate()) {}

  TDataReader(Listener<T> * l) 
    : TEntity<D<T>>(l) 
  {
    if(l)
      l->set_datareader(*this);
  } 
};

}

namespace rti {

class EntityImpl
{
protected:
  bool retained;
public:
  EntityImpl() : retained(false) {}
  virtual ~EntityImpl() {}
  void retain() { retained = true; }
};

class SubscriberImpl : public EntityImpl
{
  std::vector<EntityImpl *> datareaders;
public:

  SubscriberImpl(int i = 0) 
  {
    std::cout << "SubscriberImpl(int i = 0)\n";
  }

  ~SubscriberImpl()
  {
    std::cout << "~SubscriberImpl()\n";
  }

  void add_datareader(EntityImpl * dr) 
  {
    datareaders.push_back(dr);
  }

  void delete_datareader(EntityImpl * dr)
  {
    datareaders.erase(std::find(datareaders.begin(), datareaders.end(), dr));
  }

  void try_delete() 
  {
    if(!EntityImpl::retained && 
       datareaders.empty())
      delete this;
  }
};

template <class T>
class DataReaderImpl : public EntityImpl
{
public:
  DataReaderImpl() 
  {
    std::cout << "DataReaderImpl()\n"; 
  }

  DataReaderImpl(SubscriberImpl *sub) 
    : sub_(sub)
  {
    if(!sub)
      throw std::invalid_argument("subcriber is NULL");
    
    sub->add_datareader(this);
  }

  DataReaderImpl(dds::Listener<T> * l) 
    : l_(l) 
  {
    std::cout << "DataReaderImpl(listener *)\n"; 
  }

  ~DataReaderImpl() 
  {
    sub_->delete_datareader(this);
    std::cout << "~DataReaderImpl()\n"; 
  }

  void try_delete() 
  {
    if(!EntityImpl::retained)
      delete this;
  }

  void close() {
    EntityImpl::retained = false;
    try_delete();
  }

private:
  dds::Listener<T> * l_;
  SubscriberImpl * sub_;
};

}

namespace dds {

template <class T>
using DataReader = TDataReader<T, rti::DataReaderImpl>;

using Subscriber = TSubscriber<rti::SubscriberImpl>;

template <class T>
class Listener
{
  dds::DataReader<T> dr_;
public:
  Listener() : dr_(nullptr) {}
  void set_datareader(dds::DataReader<T> dr) { dr_ = dr; }
  void reset_datareader() { dr_ = nullptr; }
};

}


struct Foo {};

int main(void)
{
  dds::DataReader<Foo> dr1;
  {
    //dds::DataReader<Foo> dr1(new dds::Listener<Foo>());

    dds::Subscriber sub(65);
    dds::DataReader<Foo> dr2(sub);
    dr2.retain();
    dr1 = dr2;
  }
  std::cout << "out of scope\n";
  dr1.close();

}
