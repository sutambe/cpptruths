/**
 *  @file  C++ Interface: IOV_Helper
 *
 *  @brief Declares interface for IOV_Helper.
 *
 *  @author Sumant Tambe <sutambe@prince.dre.vanderbilt.edu>
 */

#ifndef __IOV_HELPER_H_
#define __IOV_HELPER_H_

#include "ace/SOCK_Stream.h"
#include <memory>

/**
 *  @class  IOV_Helper
 *
 *  @brief Encapsulates I/O vector manipulator.
 */

/** *************************************************************************/
/** ******************** IOV_Helper with Bridge pattern *********************/
/** *************************************************************************/


class IOV_Impl_Base
{
  public:
    virtual void add (void const *, size_t length) = 0; /// throws int
    virtual int send (ACE_SOCK_Stream &out) = 0;
    virtual ~IOV_Impl_Base () {};
};

template <unsigned int SIZE>
class IOV_Impl : public IOV_Impl_Base
{
  public:
    IOV_Impl ();
    virtual void add (void const *, size_t length); /// throws int
    virtual int send (ACE_SOCK_Stream &out);

  private:
    iovec iov_[SIZE];
    size_t count_;

    IOV_Impl (const IOV_Impl &);
    IOV_Impl & operator = (const IOV_Impl &);
};


template <unsigned int SIZE>
IOV_Impl<SIZE>::IOV_Impl()
: count_ (0)
{
}

template <unsigned int SIZE>
int IOV_Impl<SIZE>::send (ACE_SOCK_Stream &out)
{
  return out.sendv_n (iov_, count_);
}

template <unsigned int SIZE>
void IOV_Impl<SIZE>::add (void const *ptr, size_t length)
{
  if (count_ >= SIZE)
    throw count_;

  iov_[count_].iov_base = const_cast <void *> (ptr);
  iov_[count_].iov_len = length;
  ++count_;
}


template <unsigned int SIZE>
class IOV_Helper
{
  public:
    IOV_Helper();
    //IOV_Helper & operator = (const IOV_Helper &);
    //IOV_Helper(const IOV_Helper &);

    template <unsigned int U>
    IOV_Helper (IOV_Helper <U> const &);

    IOV_Helper <SIZE-1> add (void const *, size_t length); /// throws int
    int send (ACE_SOCK_Stream &out);
    IOV_Impl_Base *get () const;

  private:
    IOV_Impl_Base *impl_;
    //std::auto_ptr <IOV_Impl_Base> impl_;
};

template <>
class IOV_Helper <1>
{
  public:
    IOV_Helper()
    : impl_ (new IOV_Impl <1>())
    {}

    void add (void const *ptr, size_t length)
    {
        this->impl_->add (ptr, length);
    }

    int send (ACE_SOCK_Stream &out)
    {
      return this->impl_->send (out);
    }

    IOV_Helper (IOV_Helper <2> const &iov_helper)
      : impl_ (iov_helper.get())
    {}

    IOV_Impl_Base * get () const
    {
      return this->impl_;
    }

  private:
    IOV_Impl_Base *impl_;
};

template <unsigned int SIZE>
IOV_Helper<SIZE>::IOV_Helper()
: impl_ (new IOV_Impl <SIZE>())
{
}

template <unsigned int SIZE>
template <unsigned int U>
IOV_Helper<SIZE>::IOV_Helper (IOV_Helper <U> const &iov_helper)
: impl_ (iov_helper.get())
{
}

template <unsigned int SIZE>
int IOV_Helper<SIZE>::send (ACE_SOCK_Stream &out)
{
  return this->impl_->send (out);
}

template <unsigned int SIZE>
IOV_Helper<SIZE-1> IOV_Helper<SIZE>::add (void const *ptr, size_t length)
{
  this->impl_->add (ptr, length);
  return *this;
}

template <unsigned int SIZE>
IOV_Impl_Base * IOV_Helper<SIZE>::get () const
{
  return this->impl_;
}






/** *************************************************************************/
/** ************* Simple IOV_Helper without Bridge pattern ******************/
/** *************************************************************************/

/**
template <unsigned int SIZE>
class IOV_Helper
{
  public:
    IOV_Helper();
    IOV_Helper <SIZE> &add (void const *, size_t length); /// throws int
    int send (ACE_SOCK_Stream &out);

  private:
    iovec iov_[SIZE];
    size_t count_;
};

template <unsigned int SIZE>
IOV_Helper<SIZE>::IOV_Helper()
: count_ (0)
{
}

template <unsigned int SIZE>
int IOV_Helper<SIZE>::send (ACE_SOCK_Stream &out)
{
  return out.sendv_n (iov_, count_);
}

template <unsigned int SIZE>
IOV_Helper<SIZE> & IOV_Helper<SIZE>::add (void const *ptr, size_t length)
{
  if (count_ >= SIZE)
    throw count_;

  iov_[count_].iov_base = const_cast <void *> (ptr);
  iov_[count_].iov_len = length;
  ++count_;

  return *this;
}

*/

#endif /// __IOV_HELPER_H_

