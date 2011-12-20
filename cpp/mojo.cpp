#include <cstdio>
#include <memory>

namespace mojo
{
  template <class T>
  class constant // type sugar for constants
  {
    const T* data_;
  public:
    explicit constant(const T* obj) : data_(obj)
    { }

    const T* get() const
    {
      return data_;
    }
  };

  template <class T>
  class temporary : private constant<T> 
  {
  public:
    explicit temporary(T* obj) : constant<T>(obj)
    { }
    T* get() const
    {
      return const_cast<T *>(constant<T>::get());
    }
  };
  /*
  template <class T> struct enabled // inside mojo
  {
    operator temporary<T>()
    {
      return temporary<T>(static_cast<T&>(*this));
    }
    operator constant<T>() const
    {
      return constant<T>(static_cast<const T&>(*this));
    }
  protected:
    enabled() {} // intended to be derived from
    ~enabled() {} // intended to be derived from
  };
  */

} // mojo namespace 

class MoveOnlyString
{
  private:
    char * data_;

  public:
    MoveOnlyString (char * ptr = 0) throw ()
      : data_ (ptr)
    {}    
  private:  
    MoveOnlyString (MoveOnlyString &m) throw ();
    MoveOnlyString & operator = (MoveOnlyString &m) throw ();
    MoveOnlyString (mojo::constant<char> t) throw (); // The proxy move constructor
    operator mojo::constant<char> () const throw(); 
    
  public:
    MoveOnlyString (mojo::temporary<char> t) throw () // The proxy move constructor
      : data_(t.get())
    {
      printf("mojo::temporary to MoveOnlyString constructor\n");
    }
    MoveOnlyString & operator = (mojo::temporary<char> t) throw ()
    {
      MoveOnlyString temp(t);
      temp.swap (*this); // copy and swap idiom
      return *this;
    }
    void swap (MoveOnlyString &m) throw ()
    {
      std::swap (this->data_, m.data_);
    }
    operator mojo::temporary<char> () throw ()
    {
      mojo::temporary<char> t(this->data_);
      this->data_ = 0;
      printf("MoveOnlyString to mojo::temporary conversion operator\n");
      return t;
    }
    ~MoveOnlyString () throw ()
    {
      delete [] data_;
      printf("MoveOnlyString destroyed\n");
    }     
};

MoveOnlyString move(MoveOnlyString & mr)
{
  return MoveOnlyString(mojo::temporary<char>(mr));
};

MoveOnlyString move(mojo::temporary<char> t)
{
  return MoveOnlyString(t);
};

MoveOnlyString move(mojo::constant<char> t);

MoveOnlyString source()
{
  MoveOnlyString mr(new char[100]);
  return move(mr);
}

void sink(MoveOnlyString mr)
{
  printf("sink\n");
}

int main(void)
{
  // Move of Joint Objects (MOJO)
  // http://drdobbs.com/article/print?articleId=184403855&siteSectionName=#1

  source();
  MoveOnlyString mr1(new char[500]);
  sink(move(mr1));
  sink(move(move(move(source()))));
  const MoveOnlyString const_mr1(new char[20]);
  //sink(move(const_mr1)); // compiler error
}
