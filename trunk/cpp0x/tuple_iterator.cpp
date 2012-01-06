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
    size_t index;

    bool is_same_iter(tuple_storage const &ts) const {
      return (&tuple == &ts.tuple) && (index == ts.index);
    }

    T & get() const
    {
      throw std::out_of_range("Tuple iterator accessed out of valid range.");
    }

    template <size_t I>
    T & value()
    {
      return std::get<I>(tuple);
    }

  public:
    tuple_storage(Tuple &t, size_t i) 
      : tuple(t), index(i) {}
};

template <size_t I, typename T, typename Tuple>
class tuple_index : protected tuple_index<I-1, T, Tuple>
{
    typedef tuple_index<I-1, T, Tuple> Super;
  protected:
    T & get() const
    {
      if(Super::index == I) 
        //return Super::value<I>();
        return std::get<I>(Super::tuple);
      else
        return Super::get();
    }
  public:
    tuple_index(Tuple &t, size_t i) : Super(t, i) {}
};

template <typename T, typename Tuple>
class tuple_index<0, T, Tuple> : protected tuple_storage<T, Tuple>
{
    typedef tuple_storage<T, Tuple> Super;
  protected:
    T & get()const 
    {
      if(Super::index == 0) 
        //return Super::value<0>() 
        return std::get<0>(Super::tuple);
      else
        return Super::get();
    }
  public:
    tuple_index(Tuple &t, size_t i) : Super(t, i) {}
};

template <typename Tuple>
class tuple_iterator : protected tuple_index<std::tuple_size<Tuple>::value - 1,
                                             typename std::tuple_element<0, Tuple>::type,
                                             Tuple>,
                       public std::iterator <std::forward_iterator_tag,
                                             typename std::tuple_element<0, Tuple>::type>

{
   typedef typename std::tuple_element<0, Tuple>::type Head;
   enum { TUPLE_SIZE = std::tuple_size<Tuple>::value };
   typedef tuple_index<TUPLE_SIZE-1, Head, Tuple> Super;

  public:
    tuple_iterator(Tuple &t, size_t i = TUPLE_SIZE) : Super(t, i) {}
    Head & operator *() const {
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
    bool operator == (tuple_iterator const & ti) const {
      return Super::is_same_iter(ti);
    }
    bool operator != (tuple_iterator const & ti) const {
      return !(*this == ti);
    }
};

template <typename Tuple>
tuple_iterator<Tuple> begin(Tuple &t)
{
  return tuple_iterator<Tuple>(t, 0);
}

template <typename Tuple>
tuple_iterator<Tuple> end(Tuple &t)
{
  return tuple_iterator<Tuple>(t);
}

int main(void)
{
  auto tuple = std::make_tuple(10, 20, 30, 40);
  tuple_iterator<decltype(tuple)> titer(tuple);
  std::copy(begin(tuple), end(tuple), std::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;

  return 0;
}

