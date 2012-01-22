#include <tuple>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <iterator>

template <typename Tuple,
          size_t I = std::tuple_size<Tuple>::value-1>
class TupleAt
{
    typedef typename std::tuple_element<0, Tuple>::type T;

  public:
    static T & get(Tuple & tuple, size_t index)
    {
      return (index == I)? std::get<I>(tuple) : TupleAt<Tuple, I-1>::get(tuple, index);
    }
    static const T & get_const(const Tuple & tuple, size_t index)
    {
      return (index == I)? std::get<I>(tuple) : TupleAt<Tuple, I-1>::get_const(tuple, index);
    }
};

template <typename Tuple>
class TupleAt<Tuple, 0>
{
    typedef typename std::tuple_element<0, Tuple>::type T;
  public:

    static T & get(Tuple & tuple, size_t index)
    {
      return std::get<0>(tuple);
    }
    static const T & get_const(const Tuple & tuple, size_t index)
    {
      return std::get<0>(tuple);
    }
};

template <typename Tuple>
class tuple_iterator : public std::iterator <std::random_access_iterator_tag,
                                             typename std::tuple_element<0, Tuple>::type>

{
    typedef typename std::tuple_element<0, Tuple>::type T;
    enum { TUPLE_SIZE = std::tuple_size<Tuple>::value };
   
    Tuple * tuple;
    int index;

 public:
  
    typedef int difference_type; 

    explicit tuple_iterator(Tuple & t, int i = TUPLE_SIZE) 
      : tuple(&t), index(i) 
    {}
    T & operator *()
    {
      return TupleAt<Tuple>::get(*tuple, index);
    }
    T * operator ->()
    {
      return & TupleAt<Tuple>::get(*tuple, index);
    }
    T & operator [] (int offset)
    {
      return TupleAt<Tuple>::get(*tuple, index+offset);
    }
    tuple_iterator & operator ++ () 
    {
      if(index < TUPLE_SIZE)
        ++index;
      return *this;
    }
    tuple_iterator operator ++ (int) 
    {
      tuple_iterator temp(*this);
      ++(*this);
      return temp;
    }
    tuple_iterator & operator -- () 
    {
      if(index >= 0)
        --index;
      return *this;
    }
    tuple_iterator operator -- (int) 
    {
      tuple_iterator temp(*this);
      --(*this);
      return temp;
    }
    tuple_iterator operator - (int i)
    {
      tuple_iterator temp(*tuple, index-i);
      return temp;
    }
    tuple_iterator & operator -= (int i)
    {
      index-=i;
      return *this;
    }
    tuple_iterator operator + (int i)
    {
      tuple_iterator temp(*tuple, index+i);
      return temp;
    }
    tuple_iterator & operator += (int i)
    {
      index+=i;
      return *this;
    }
    difference_type operator - (const tuple_iterator & ti)
    {
      return index - ti.index;
    }
    bool operator < (const tuple_iterator &ti)
    {
      return index < ti.index;
    }
    bool operator > (const tuple_iterator &ti)
    {
      return index > ti.index;
    }
    bool operator <= (const tuple_iterator &ti)
    {
      return index <= ti.index;
    }
    bool operator >= (const tuple_iterator &ti)
    {
      return index >= ti.index;
    }
    bool operator == (tuple_iterator const & ti) const {
      return (tuple == ti.tuple) && (index == ti.index);
    }
    bool operator != (tuple_iterator const & ti) const {
      return !(*this == ti);
    }
};

template <>
class tuple_iterator <std::tuple<>> 
{
    void operator * ();
    tuple_iterator & operator ++ ();
    tuple_iterator   operator ++ (int);
    tuple_iterator & operator -- ();
    tuple_iterator   operator -- (int);

  public:
    tuple_iterator(std::tuple<>, size_t i = 0) {}
};

template <typename... Args>
tuple_iterator<std::tuple<Args...>> begin(std::tuple<Args...> &t)
{
  return tuple_iterator<std::tuple<Args...>>(t, 0);
}

template <typename... Args>
tuple_iterator<std::tuple<Args...>> end(std::tuple<Args...> &t)
{
  return tuple_iterator<std::tuple<Args...>>(t);
}

#define USING(Dervied, Base)                                 \
      template<typename ...Args,                             \
             typename = typename std::enable_if              \
             <                                               \
                std::is_constructible<Base, Args...>::value  \
             >::type>                                        \
    Dervied(Args &&...args)                                  \
        : Base(std::forward<Args>(args)...) { }              \

template <typename... T>
class tuple_array : public std::tuple<T...>
{
    typedef std::tuple<T...> Tuple;
    typedef typename std::tuple_element<0, Tuple>::type HeadType;
    enum { TUPLE_SIZE = std::tuple_size<Tuple>::value };

  public:
    USING(tuple_array, Tuple);

    HeadType & operator [] (size_t index) 
    {
      return TupleAt<Tuple>::get(*this, index);
    }
    HeadType & at(size_t index) 
    {
      if(index >= TUPLE_SIZE)
        throw std::out_of_range("Tuple iterator dereferenced out of valid range."); 
      else
        return TupleAt<Tuple>::get(*this, index);
    }
    const HeadType & operator [] (size_t index) const
    {
      return TupleAt<Tuple>::get_const(*this, index);
    }
    const HeadType & at(size_t index) const 
    {
      if(index >= TUPLE_SIZE)
        throw std::out_of_range("Tuple iterator dereferenced out of valid range."); 
      else
        return TupleAt<Tuple>::get_const(*this, index);
    }
};

template <typename... T>
tuple_array<T...> make_tuple_array(T... args)
{
  return { std::forward<T&&>(args)... };
}

int main(void)
{
  auto tuple = std::make_tuple(10, 20, 30, 40);
  tuple_iterator<decltype(tuple)> titer(tuple);
  std::copy(begin(tuple), end(tuple), std::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;
  auto ta = make_tuple_array(4, 2, 1);
  std::cout << "sizeof(ta) = " << sizeof(ta) << std::endl;
  std::cout << ta[0] << std::endl;
  std::cout << ta[1] << std::endl;
  std::sort(begin(ta), end(ta));

  for(int i : ta)
  {
    std::cout << i <<std::endl;
  }

  return 0;
}

