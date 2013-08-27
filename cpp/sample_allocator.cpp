#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

#include <stdlib.h>
#include <stddef.h>
#include <time.h>

#define simple_alignof(T) \
(sizeof (T) > 1 ? offsetof(align::offset<T>, x) : 1)

#define ALIGNOF(T) (std::min(sizeof(T), simple_alignof(T)))

#define BUF_SIZE (1024*8)

namespace align {
  template <class T>
  struct offset
  {
    char c;
    T x;
  };
}

struct InvalidBuf {};

class BufferPool
{
  std::vector<void *> avail;
  std::vector<void *> loaned;
  
  BufferPool(unsigned int buf_size,
             unsigned int pool_size)
    : avail(pool_size),
      loaned()
  {
    loaned.reserve(pool_size);
    for(unsigned int i=0;i < pool_size; ++i)
    {
      avail[i] = malloc(buf_size);
      
    }
  }  

public:  

  static BufferPool & instance()
  {
    static BufferPool pool(BUF_SIZE, 20);
    return pool;
  }

  void * getbuf()
  {
    if(!avail.empty())
    {
      void * buf = avail.back();
      loaned.push_back(buf);
      avail.pop_back();
      return buf;
    }
    else
      throw std::bad_alloc();
  }
  
  void return_buf(void * buf)
  {
    std::vector<void *>::iterator it = 
      std::find(loaned.begin(), loaned.end(), buf);
      
    if(it != loaned.end())
    {
      loaned.erase(it);
      avail.push_back(buf);
    }
    else
    {
      throw InvalidBuf();
    }
  }
  
  ~BufferPool()
  {
    for(unsigned int i = 0;i < avail.size(); ++i)
    {
      free(avail[i]);
    }
    for(unsigned int i = 0;i < loaned.size(); ++i)
    {
      free(loaned[i]);
    }
  }
};

struct AllocState
{
  void * buf;
  std::size_t size;  
  void * static_end; // one past the last byte of T.
  void * tail;
};

#ifdef STD_ALLOC

template <class T>
class SampleAllocator : public std::allocator<T>
{
public:

  template <class Other>
  struct rebind
  {	
    typedef SampleAllocator<Other> other;
  };

  explicit SampleAllocator(AllocState * = 0) throw()
  {}

  SampleAllocator(const SampleAllocator & sa) throw()
  {	}

  template <class Other>
  SampleAllocator(const SampleAllocator<Other> &sa) throw()
  {	}

  template<class Other>
  SampleAllocator & operator = (const SampleAllocator<Other> & sa)
  {
    std::allocator<T>::operator = (*this, sa);
    return *this;
  }

};

#else

template <class T>
class SampleAllocator
{	
public:
  AllocState * state_ptr;

  typedef T value_type;

  typedef value_type * pointer;
  typedef value_type & reference;
  typedef const value_type * const_pointer;
  typedef const value_type & const_reference;

  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;

  template <class Other>
  struct rebind
  {	
    typedef SampleAllocator<Other> other;
  };

  pointer address(reference val) const
  {
    // val may have overloaded &. So cast to char & first.
    return ((pointer) &(char&) val);
  }

  const_pointer address(const_reference val) const
  {
    return ((const_pointer) &(char&)val);
  }

  explicit SampleAllocator(AllocState * ptr) throw()
    : state_ptr(ptr)
  {	}
  
  SampleAllocator(const SampleAllocator & sa) throw()
    : state_ptr(sa.state_ptr)
  {	}

  template <class Other>
  SampleAllocator(const SampleAllocator<Other> &sa) throw()
    : state_ptr(sa.state_ptr)
  {	}

  template<class Other>
  SampleAllocator & operator = (const SampleAllocator<Other> & sa)
  {	// assign from a related allocator (do nothing)
    this->state_ptr = sa.state_ptr;
    return (*this);
  }

  void deallocate(pointer ptr, size_type)
  {	
    // no-op!
    // deallocate is called for each stl container member when it
    // goes out of scope. I.e., when the sample goes out of
    // scope. We don't do anything because Sample<T> will return 
    // the entire buffer to the BufferPool. As a result this 
    // allocator cannot grow/shrink memory.
  }

  pointer allocate(size_type count, const void * = 0)
  {	
    // Allocate array of count elements. Ignore hint
    // Alignment must be a factor of sizeof(T)
    // The return address must be divisible by alignment
#ifdef _DEBUG
    std::string str = typeid(T).name();
    size_t sizeT = sizeof(T);
#endif

    if(count)
    {
      size_type alignment = ALIGNOF(T);
      char * new_tail = (char *) state_ptr->tail;
      while(unsigned int(new_tail) % alignment) --new_tail;
      new_tail -= (sizeof(T) * count);
      
      if(new_tail < state_ptr->static_end)
        throw std::bad_alloc();
      else
        state_ptr->tail = new_tail;
    }
    return (T *) state_ptr->tail;
  }

  void construct(pointer ptr, const T & val)
  {	// construct object at ptr with value val
    new ((void *)ptr) T(val);
  }

#ifdef CPP11
  void construct(pointer ptr, T && val)
  {	// construct object at _Ptr with value val
    ::new ((void *)ptr) T(std::forward<T>(val));
  }

  template<class Other>
  void construct(pointer ptr, Other && val)
  {	// construct object at ptr with value val
    new ((void *)ptr) T(std::forward<Other>(val));
  }
#endif

  void destroy(pointer ptr)
  {	// destroy object at ptr
    if(ptr)
      ptr->~T();
  }

  size_type max_size() const throw()
  {	// estimate maximum array size
    return size_type(((char *)(state_ptr->buf) + state_ptr->size) - (char *)state_ptr->static_end);
  }
};

#endif // ifdef STD_ALLOC

typedef std::basic_string<char, std::char_traits<char>, SampleAllocator<char> > RTIString;

template <class T>
struct RTIVector
{
  typedef std::vector<T, SampleAllocator<T> > type;
};

std::ostream & operator << (std::ostream & o, const RTIString & s)
{
  o << s.c_str();
  return o;
}

struct Bar
{
  RTIString name_;
  RTIString id_;
  double d_;
  RTIVector<double>::type vd_;

  Bar(AllocState * ptr)
    : name_(SampleAllocator<char>(ptr)),
      id_(SampleAllocator<char>(ptr)),
      vd_(SampleAllocator<double >(ptr))
  { }

  friend std::ostream & operator << (std::ostream & o, const Bar & b)
  {
    o << "printing Bar\n";
    o << b.name_ << ":" << b.id_ << ":" << b.d_ << ":" << std::endl;
    std::copy(b.vd_.begin(), b.vd_.end(), std::ostream_iterator<double>(o, " "));
    o << std::endl;
    return o;
  }
};

struct Foo
{
  RTIString color_;
  int x_;
  int y_;
  int shapesize_;
  RTIVector<int>::type vi_;
  Bar bar_;
  RTIVector<Bar>::type vbar_;

  Foo(AllocState * ptr)
    : color_(SampleAllocator<char>(ptr)),
      vi_(SampleAllocator<int>(ptr)),
      bar_(ptr),
      vbar_(SampleAllocator<Bar>(ptr))
  { }

  friend std::ostream & operator << (std::ostream & o, const Foo & f)
  {
    o << "printing Foo\n";
    o << f.color_ << ":" << f.x_ << ":" << f.y_ << ":" << f.shapesize_ << std::endl;
    std::copy(f.vi_.begin(), f.vi_.end(), std::ostream_iterator<int>(o, " "));
    o << std::endl;
    o << f.bar_;
    std::copy(f.vbar_.begin(), f.vbar_.end(), std::ostream_iterator<Bar>(o));
    return o;
  }
};

template <class T, class Alloc>
std::ostream & operator << (std::ostream & o, const std::vector<T, Alloc> & v)
{
  for(std::vector<T, Alloc>::const_iterator iter = v.begin();
      iter != v.end();
      ++iter)
  {
    o << *iter << std::endl;
  }
  return o;
}
 
template <class T>
AllocState * overlay(void * buf, size_t bufsize)
{
  AllocState * alloc_state = new (buf) AllocState;
  alloc_state->buf = buf;
  alloc_state->size = bufsize;
  alloc_state->static_end = (char *) buf + sizeof(AllocState) + sizeof(T);
  alloc_state->tail = (char *) buf + bufsize;
  
  return alloc_state;
}

#ifdef STD_ALLOC

template <class T>
class Sample
{
  T * t;

  Sample(const Sample &);
  Sample & operator = (const Sample &);

public:
  Sample()
    : t(new T(0))
  { }

  T * get_value() const
  {
    return t;
  }

  ~Sample()
  {
    delete t;
  }
};

#else

template <class T>
class Sample
{
  void * buf;
  
  Sample(const Sample &);
  Sample & operator = (const Sample &) ;

public:
  Sample()
    : buf(BufferPool::instance().getbuf())
  {
    overlay<T>(buf, BUF_SIZE);
    new ((char *) buf + sizeof(AllocState)) T((AllocState *) buf);
  }

  T * get_value() const
  {
    return (T *) ((char *) buf + sizeof(AllocState));
  }

  AllocState * alloc_state() const
  {
    return (AllocState *) buf;
  }

  ~Sample()
  {
    BufferPool::instance().return_buf(buf);
  }
};

#endif // STD_ALLOC

// class generator:
struct incrementor {
  int current;
  incrementor(int start = 0) { current = start; }
  int operator()() { return ++current; }
} ;

int main(void)
{
  try {
    const char * str = "BLUE";
    int x = 10, y = 20, shapesize = 30;
    const int SIZE = 20;
    int data[SIZE] = { 1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 
                      11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
    
    std::generate(data, data + SIZE, incrementor(0));
    
    BufferPool::instance();

    time_t t1, t2;
    time(&t1);

    for (int i = 0; i < 1000000; ++i)
    {
      Sample<Foo> foo_sample;
      AllocState * alloc_state = 0;

#ifndef STD_ALLOC
      alloc_state = foo_sample.alloc_state();
#endif

      Foo * foo = foo_sample.get_value();

      foo->color_ = str;
      foo->x_ = x;
      foo->y_ = y;
      foo->shapesize_ = shapesize;
      foo->vi_.reserve(SIZE);
      std::copy(data, data + SIZE, std::back_inserter(foo->vi_));

      foo->bar_.name_ = "RTI-RTI-RTI-RTI-RTI-RTI-RTI-RTI-RTI-RTI";
      foo->bar_.id_ = "ID";
      foo->bar_.d_ = 99.99;
      foo->bar_.vd_.reserve(SIZE);
      std::copy(data, data + SIZE, std::back_inserter(foo->bar_.vd_));

      foo->vbar_.reserve(SIZE);
      for (int i = 0; i < SIZE; ++i)
      {
        Bar b(alloc_state);
        foo->vbar_.push_back(b);
        foo->vbar_[i].name_ = "RTI-RTI-RTI-RTI-RTI-RTI-RTI-RTI-RTI-RTI-RTI-RTI";
        foo->vbar_[i].id_ = "ID";
        foo->vbar_[i].d_ = 99.99;
        foo->vbar_[i].vd_.reserve(SIZE);
        std::copy(data, data + SIZE, std::back_inserter(foo->vbar_[i].vd_));
      }

      //std::cout << *foo;
    }

    time(&t2);
    std::cout << "Time difference = " << t2 - t1 << std::endl;
  }
  catch (std::bad_alloc & ex)
  {
    std::cerr << "Memory allocation error: " 
              << ex.what() 
              << std::endl;
  }
  return 0;
}


/*
template <class T>
class MFAllocoator
{
public:
typedef T value_type;

typedef value_type * pointer;
typedef value_type & reference;
typedef const value_type * const_pointer;
typedef const value_type & const_reference;

typedef std::size_t size_type;
typedef std::ptrdiff_t difference_type;

template <class Other>
struct rebind
{
typedef MFAllocoator<Other> other;
};

pointer address(reference val) const
{
// val may have overloaded &. So cast to char & first.
return ((pointer) &(char&) val);
}

const_pointer address(const_reference val) const
{
return ((const_pointer) &(char&)val);
}

MFAllocoator() throw()
{ }

MFAllocoator(const MFAllocoator &) throw()
{ }

template <class Other>
MFAllocoator(const MFAllocoator<Other>&) throw()
{ }

template<class Other>
MFAllocoator & operator = (const MFAllocoator<Other> &)
{	// assign from a related allocator (do nothing)
return (*this);
}

void deallocate(pointer ptr, size_type)
{	// deallocate ptr, ignore size
free(ptr);
}

pointer allocate(size_type count, const void * = 0)
{	// allocate array of count elements, ignore hint
pointer p = (pointer) malloc(sizeof(T) * count);
return p? p : throw std::bad_alloc();
}

void construct(pointer ptr, const T & val)
{	// construct object at ptr with value val
::new ((void *)ptr) T(val);
}

#ifdef CPP11
void construct(pointer ptr, T && val)
{	// construct object at _Ptr with value val
::new ((void *)ptr) T(std::forward<T>(val));
}

template<class Other>
void construct(pointer ptr, Other && val)
{	// construct object at ptr with value val
::new ((void *)ptr) T(std::forward<Other>(val));
}
#endif

void destroy(pointer ptr)
{	// destroy object at ptr
if(ptr)
ptr->~T();
}

size_type max_size() const throw()
{	// estimate maximum array size
size_type count = (size_type)(-1) / sizeof (T);
return (0 < count ? count : 1);
}
};
*/