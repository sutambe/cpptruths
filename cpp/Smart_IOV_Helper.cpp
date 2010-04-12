#include <unistd.h>
#include <string.h>
#include <stdexcept>

#define STDOUT 1

typedef int ACE_Time_Value;

struct iovec
{
  char * iov_base;
  ssize_t iov_len;
};

class ACE_SOCK_IO
{
  public:
    ssize_t sendv (iovec const *v, size_t length, ACE_Time_Value const * wait)
    {
      ssize_t nbytes = 0;
      for (size_t i = 0;i < length; ++i)
      {
        nbytes += write (STDOUT, v[i].iov_base, v[i].iov_len);
      }
      write (STDOUT, "\n", 1);
      return (nbytes >= 10)? 10 : nbytes;
      
    }
};


template <size_t N>
class IOVHelper
{
    iovec iov_[N];
    size_t count_;
    size_t offset_;
    size_t total_size_;
    size_t total_sent_bytes_;
  public:
    IOVHelper ()
      : count_ (0),
        offset_ (0),
        total_size_ (0),
        total_sent_bytes_ (0)
    {}
    int add (char * base, size_t size)
    {
      if (count_ < N)
      {
         iov_[count_].iov_base = base;
         iov_[count_].iov_len  = size;
         ++count_;
         total_size_ += size;
         return count_ - 1;
      }
      else
      {
        throw std::range_error ("iovector bounds exceeded.");
      }
    }
    ssize_t sendv (ACE_SOCK_IO & sock, ACE_Time_Value const * wait)
    {
      ssize_t sent_bytes = sock.sendv (iov_ + offset_, count_ - offset_, wait);
      if (sent_bytes <= 0)
      {
        return sent_bytes;
      }
      total_sent_bytes_ += sent_bytes;
      if (sent_all ())
      {
        return total_sent_bytes_;
      }
      else
      {
        for (size_t i = offset_;(i < count_) && (sent_bytes); ++i)
        {
          if (iov_[i].iov_len < static_cast <size_t> (sent_bytes))
          {
            sent_bytes -= iov_[i].iov_len;
            iov_[i].iov_len = 0;
          }
          else
          {
            iov_[i].iov_base += static_cast <char *> (iov_[i].iov_base)
                                + sent_bytes;
            iov_[i].iov_len -= sent_bytes;
            sent_bytes = 0;
            offset_ = iov_[i].iov_len ? i : i+1;
          }
        }
        return total_sent_bytes_;
      }
    }
    ssize_t total_size () const
    {
      return total_size_;
    }
    bool sent_all () const
    { 
      return (total_sent_bytes_ == total_size_);
    }
};

int main (void)
{
  char * str1 = "";
  char * str2 = "Sumant";
  char * str3 = "";
  char * str4 = "Tambe";
  IOVHelper <4> iov_sender;

  iov_sender.add (str1, strlen (str1));
  iov_sender.add (str2, strlen (str2));
  iov_sender.add (str3, strlen (str3));
  iov_sender.add (str4, strlen (str4));
  
  ACE_Time_Value wait = 5;
  ACE_SOCK_IO io;
  while (! iov_sender.sent_all ())
    iov_sender.sendv (io, &wait);

  return 0;
}

