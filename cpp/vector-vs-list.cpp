#include <iostream>
#include <vector>
#include <list>
#include <iterator>
#include <string>
#include <stdlib.h>
#include <ctime>

int gen_random()
{
  return rand() % 10000;
}

template <class FWIter>
void print(FWIter begin, FWIter end)
{
  std::copy(begin, end, std::ostream_iterator<typename FWIter::value_type>(std::cout, " "));
  std::cout << std::endl;
}

void list_insert_sort(size_t N)
{
  typedef std::list<int> T;
  T list;
  list.push_back(gen_random());
  size_t count = 1;

  print(list.begin(), list.end());

  for(size_t i = 1;i < N;++i)
  {
    int value = gen_random();

    T::iterator iter = list.begin();
    for(size_t pos = 0; (*iter < value) && (pos < count); ++iter, ++pos);
    T::iterator location = iter;

    list.insert(iter, value);

    ++count;
    print(list.begin(), list.end());
  }
}

template<typename T>
class ArrayAllocator {
public : 
    //    typedefs
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

public : 
    //    convert an ArrayAllocator<T> to ArrayAllocator<U>
    template<typename U>
    struct rebind {
        typedef ArrayAllocator<U> other;
    };

    void * buf;
    size_t current;
    
public : 
    inline explicit ArrayAllocator(size_t N) 
    {
      current=0;
      buf = malloc(sizeof(T) * N);
      if(!buf)
        throw std::runtime_error("Allocator could not allocate.");
    }
    inline ~ArrayAllocator() 
    {
      free(buf);
    }
    inline ArrayAllocator(ArrayAllocator const&) {}
    template<typename U>
    inline ArrayAllocator(ArrayAllocator<U> const&) {}

    //    address
    inline pointer address(reference r) { return &r; }
    inline const_pointer address(const_reference r) { return &r; }

    //    memory allocation
    inline pointer allocate(size_type cnt, 
       void * = 0) {  
      //return reinterpret_cast<pointer>(::operator new(cnt * sizeof (T))); 
      size_t offset = cnt * sizeof(T);
      void * ptr = ((char *)buf + current);
      current += offset;
      return static_cast<pointer>(ptr);      
    }
    inline void deallocate(pointer p, size_type) { 
        //::operator delete(p); 
    }

    //    size
    inline size_type max_size() const { 
        return std::numeric_limits<size_type>::max() / sizeof(T);
    }

    //    construction/destruction
    inline void construct(pointer p, const T& t) { new(p) T(t); }
    inline void destroy(pointer p) { p->~T(); }

    inline bool operator==(ArrayAllocator const&) { return true; }
    inline bool operator!=(ArrayAllocator const& a) { return !operator==(a); }
};    //    end of class ArrayAllocator 

void list_arrayalloc_insert_sort(size_t N)
{
  typedef std::list<int, ArrayAllocator<int> > T;
  T list(N, 0, ArrayAllocator<int>(N));
  *(list.begin()) = gen_random();
  size_t count = 1;

  print(list.begin(), list.end());

  for(size_t i = 1;i < N;++i)
  {
    int value = gen_random();

    T::iterator iter = list.begin();
    for(size_t pos = 0; (*iter < value) && (pos < count); ++iter, ++pos);
    T::iterator location = iter;

    *iter = value;

    ++count;
    print(list.begin(), list.end());
  }
}

void vector_insert_sort(size_t N)
{
  typedef std::vector<int> T;
  T vector;
  vector.reserve(N);
  vector.push_back(gen_random());
  size_t count = 1;

  print(vector.begin(), vector.end());

  for(size_t i = 1;i < N;++i)
  {
    int value = gen_random();

    T::iterator iter = vector.begin();
    for(size_t pos = 0; (*iter < value) && (pos < count); ++iter, ++pos);
    T::iterator location = iter;

    if(iter == vector.end())
      vector.push_back(value);
    else
    {
      vector.resize(vector.size()+1);
      int v1 = *iter;
      do
      {
        int v2 = *(iter+1);
        *(iter+1) = v1;
        ++iter;
        v1 = v2;
      }
      while((iter+1) != vector.end());
      
      *location = value;
    }

    ++count;
    print(vector.begin(), vector.end());
  }
}

int main(int argc, char * argv[])
{
  time_t t;
  time(&t);
  srand(t);

  if(argc <= 2) {
    std::cout << "Usage: vector-vs-list <size> <list|vector>\n";
    return 1;
  }

  if(std::string(argv[2]) == "list")
    list_insert_sort(atoi(argv[1]));
  else if(std::string(argv[2]) == "list_arrayalloc")
    list_arrayalloc_insert_sort(atoi(argv[1]));
  else  if(std::string(argv[2]) == "vector")
    vector_insert_sort(atoi(argv[1]));
  else
    std::cout << "Don't know what to run\n";

  return 0;
}
