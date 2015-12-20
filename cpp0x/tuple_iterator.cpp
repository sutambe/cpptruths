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
      if(index == 0)
       return std::get<0>(tuple);
      else
       throw std::out_of_range("Tuple iterator dereferenced out of valid range.");
    }
    static const T & get_const(const Tuple & tuple, size_t index)
    {
      if(index == 0)
       return std::get<0>(tuple);
      else
       throw std::out_of_range("Tuple iterator dereferenced out of valid range.");

    }
};

template <typename Tuple>
class tuple_iterator : public std::iterator <std::random_access_iterator_tag,
                                             typename std::tuple_element<0, Tuple>::type>

{
    typedef typename std::tuple_element<0, Tuple>::type T;
    enum { TUPLE_SIZE = std::tuple_size<Tuple>::value };
   
    Tuple * tuple;
    int current_;
    int last_;
    T * ref_;

    T * update_ref()
    {
      if(current_ != last_)
      {
        ref_ = & TupleAt<Tuple>::get(*tuple, current_);
        last_ = current_;
      }
      if(!ref_)
        throw std::out_of_range("Tuple iterator dereferenced out of valid range.");
      
      return ref_;
    }

 public:
  
    typedef int difference_type; 

    explicit tuple_iterator(Tuple & t, size_t i = TUPLE_SIZE) 
      : tuple(&t), 
        current_(i), 
        last_(i), 
        ref_((i==TUPLE_SIZE)? nullptr : &TupleAt<Tuple>::get(*tuple, last_)) 
    {}
    T & operator *() {
      return *update_ref();
    }
    T * operator ->() {
      return update_ref();
    }
    T & operator [] (int offset) {
      return TupleAt<Tuple>::get(*tuple, current_+offset);
    }
    tuple_iterator & operator ++ () {
      if(current_ < TUPLE_SIZE)
        ++current_;
      return *this;
    }
    tuple_iterator operator ++ (int) {
      tuple_iterator temp(*this);
      ++(*this);
      return temp;
    }
    tuple_iterator & operator -- () {
      if(current_ >= 0)
        --current_;
      return *this;
    }
    tuple_iterator operator -- (int) {
      tuple_iterator temp(*this);
      --(*this);
      return temp;
    }
    tuple_iterator operator - (int i) const {
      tuple_iterator temp(*tuple, current_-i);
      return temp;
    }
    tuple_iterator & operator -= (int i) {
      current_-=i;
      return *this;
    }
    tuple_iterator operator + (int i) const {
      tuple_iterator temp(*tuple, current_+i);
      return temp;
    }
    tuple_iterator & operator += (int i) {
      current_+=i;
      return *this;
    }
    difference_type operator - (const tuple_iterator & ti) const {
      return current_ - ti.current_;
    }
    bool operator < (const tuple_iterator &ti) const {
      return current_ < ti.current_;
    }
    bool operator > (const tuple_iterator &ti) const {
      return current_ > ti.current_;
    }
    bool operator <= (const tuple_iterator &ti) const {
      return current_ <= ti.current_;
    }
    bool operator >= (const tuple_iterator &ti) const {
      return current_ >= ti.current_;
    }
    bool operator == (tuple_iterator const & ti) const {
      return (tuple == ti.tuple) && (current_ == ti.current_);
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
        : Base(std::forward<Args>(args)...)                  \

template <typename... T>
class tuple_array : public std::tuple<T...>
{
    typedef std::tuple<T...> Tuple;
    typedef typename std::tuple_element<0, Tuple>::type HeadType;
    enum { TUPLE_SIZE = std::tuple_size<Tuple>::value };

    mutable HeadType * ref_;
    mutable size_t last_;

  public:
    USING(tuple_array, Tuple)
    {
      ref_ = & TupleAt<Tuple>::get(*this, TUPLE_SIZE-1);
      last_ = TUPLE_SIZE-1;
    }

    HeadType & operator [] (size_t index) 
    {
      if(last_ != index)
      {
        ref_ = & TupleAt<Tuple>::get(*this, index);
        last_ = index;
      }
      return *ref_;
    }
    const HeadType & operator [] (size_t index) const
    {
      if(last_ != index)
      {
        ref_ = & TupleAt<Tuple>::get_const(*this, index);
        last_ = index;
      }
      return *ref_;
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
  std::cout << ta[0] << " " << ta[1] << " " << ta[2] << std::endl;
  std::sort(begin(ta), end(ta));

  for(int i : ta)
  {
    std::cout << i <<std::endl;
  }

  return 0;
}

