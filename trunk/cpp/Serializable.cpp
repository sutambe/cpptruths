#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <complex>
#include <iterator>

class ISerializable
{
  public:
    virtual int serialize (char *& buffer, size_t & buf_size) const = 0;
    virtual ~ISerializable() {};
};

class X
{
  char name [10] ;
  public:
    X () 
    { 
      std::fill (name, name+10, 0);
      strcpy(name, "ABCD");
    }
};

template <class T>
class Serializable : public T,   /// Parameterized Base Class Idiom
                     public ISerializable
{
  public:
    Serializable (const T &t = T()) : T(t) {}
    virtual int serialize (char *& buffer, size_t & buf_size) const
    {
        //const size_t size = sizeof (Serializable<T>);
        const size_t size = sizeof (T);
        if (size <= buf_size)
        {
           memcpy (buffer, static_cast<const T *>(this), size);
           buffer += size;
           buf_size -= size;
           return size;
        }
        return -1; 
    }
};

int main (void)
{
  std::vector<Serializable<X> > xvect (2);
  size_t size = 1000, length = 0;
  char *buffer = new char [size];
  char *begin = buffer;

  for (std::vector<Serializable<X> >::const_iterator iter = xvect.begin();
       iter != xvect.end();
       ++iter)
  {
    length += iter->serialize(buffer, size);
  }
  std::copy (begin, begin + length, std::ostream_iterator<int> (std::cout," "));
  std::copy (begin, begin + length, std::ostream_iterator<char> (std::cout));
  return 0;
}
