#include <tuple>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <iterator>

template <typename T, typename Tuple>
class tuple_storage
{
  protected:
    Tuple & tuple;
    int index;

    bool is_same_iter(tuple_storage const &ts) const {
      return (&tuple == &ts.tuple) && (index == ts.index);
    }

    T & get() const {
      throw std::out_of_range("Tuple iterator accessed out of valid range.");
    }

  public:
    tuple_storage(Tuple &t, int i) 
      : tuple(t), index(i) {}
};

template <size_t I, typename T, typename Tuple>
class tuple_index : protected tuple_index<I-1, T, Tuple>
{
    typedef tuple_index<I-1, T, Tuple> Super;
  protected:
    T & get() const {
      return (Super::index == I) ? std::get<I>(Super::tuple) : Super::get();
    }
  public:
    tuple_index(Tuple &t, int i) : Super(t, i) {}
};

template <typename T, typename Tuple>
class tuple_index<0, T, Tuple> : protected tuple_storage<T, Tuple>
{
    typedef tuple_storage<T, Tuple> Super;
  protected:
    T & get()const {
      return (Super::index == 0) ? std::get<0>(Super::tuple) : Super::get();
    }
  public:
    tuple_index(Tuple &t, int i) : Super(t, i) {}
};

template <typename Tuple>
class tuple_iterator : protected tuple_index<std::tuple_size<Tuple>::value - 1,
                                             typename std::tuple_element<0, Tuple>::type,
                                             Tuple>,
                       public std::iterator <std::bidirectional_iterator_tag,
                                             typename std::tuple_element<0, Tuple>::type>

{
   typedef typename std::tuple_element<0, Tuple>::type T;
   enum { TUPLE_SIZE = std::tuple_size<Tuple>::value };
   typedef tuple_index<TUPLE_SIZE-1, T, Tuple> Super;

  public:
    tuple_iterator(Tuple &t, int i = TUPLE_SIZE) : Super(t, i) {}
    T & operator *() const {
      return Super::get();
    }
    tuple_iterator & operator ++ () 
    {
      if(Super::index < TUPLE_SIZE)
        ++Super::index;
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
      if(Super::index >= 0)
        --Super::index;
      return *this;
    }
    tuple_iterator operator -- (int) 
    {
      tuple_iterator temp(*this);
      --(*this);
      return temp;
    }
    bool operator == (tuple_iterator const & ti) const {
      return Super::is_same_iter(ti);
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

int main(void)
{
  auto tuple = std::make_tuple(10, 20, 30, 40);
  tuple_iterator<decltype(tuple)> titer(tuple);
  std::copy(begin(tuple), end(tuple), std::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;

  return 0;
}

