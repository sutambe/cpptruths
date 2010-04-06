#include <cstdlib>
#include <iostream>
#include <iterator>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <stdexcept>

#include <boost/tuple/tuple.hpp> 
#include <boost/tuple/tuple_io.hpp> 
#include <boost/type_traits.hpp>
#include <boost/bind.hpp>

using namespace boost;
using namespace boost::tuples;

template <class TupleOfVectors> 
struct GetTransposeCons;

template <>
struct GetTransposeCons<tuples::null_type>
{
  typedef tuples::null_type type;
};

template <class TupleOfVectors>
struct GetTransposeCons
{
  typedef typename TupleOfVectors::head_type Head;
  typedef typename TupleOfVectors::tail_type Tail;
  typedef typename
    tuples::cons<typename remove_reference<Head>::type::value_type,
                 typename GetTransposeCons<Tail>::type> type;
};

template <class T>
struct GetTransposeTuple;

template <class T0> 
struct GetTransposeTuple<cons<T0, null_type> >   
{
  typedef tuples::tuple<T0> type;
};

template <class T0, class T1> 
struct GetTransposeTuple<cons<T0, cons<T1, null_type> > >  
{
  typedef tuples::tuple<T0, T1> type;
};

template <class T0, class T1, class T2> 
struct GetTransposeTuple<cons<T0, cons<T1, cons<T2, null_type> > > > 
{
  typedef tuples::tuple<T0, T1, T2> type;
};

template <class T0, class T1, class T2, class T3> 
struct GetTransposeTuple<cons<T0, cons<T1, cons<T2, cons<T3, null_type> > > > > 
{
  typedef tuples::tuple<T0, T1, T2, T3> type;
};

template <class T0, class T1, class T2, class T3, class T4> 
struct GetTransposeTuple<cons<T0, cons<T1, cons<T2, cons<T3, cons<T4, null_type> > > > > > 
{
  typedef tuples::tuple<T0, T1, T2, T3, T4> type;
};


template <class TupleOfVectors, 
          class ValueTypeTuple = typename GetTransposeTuple<typename GetTransposeCons<TupleOfVectors>::type>::type,
          unsigned int TUPLE_INDEX = 0> 
struct Transposer 
  : Transposer <typename TupleOfVectors::tail_type,
                ValueTypeTuple,
                TUPLE_INDEX + 1>
{
  typedef typename TupleOfVectors::head_type Head;
  typedef typename TupleOfVectors::tail_type Tail;
  typedef typename remove_reference<Head>::type HeadContainer;
  typedef Transposer<Tail, ValueTypeTuple, TUPLE_INDEX + 1> super;
  typedef ValueTypeTuple TransposeTuple;
  typedef std::vector<ValueTypeTuple> Transpose;
  
  Transposer(TupleOfVectors const & tuple)
    : super(tuple.get_tail()), 
      head_container_(tuple.get_head()),
      head_iter_(head_container_.begin())
  {}
  
  Transpose get_transpose () 
  {
    Transpose tran;
    tran.reserve(head_container_.size());
    for(typename HeadContainer::const_iterator iter = head_container_.begin();
        iter != head_container_.end();
        ++iter)
    {
      ValueTypeTuple vtuple;
      this->populate_tuple(vtuple);  
      tran.push_back(vtuple);
    }
    return tran;
  }

private:

  HeadContainer const & head_container_;
  typename HeadContainer::const_iterator head_iter_;

protected:

  void populate_tuple(ValueTypeTuple & vtuple)
  {
    if(head_iter_ == head_container_.end())
      throw std::runtime_error("Container bound exceeded.");
    else
    {
      vtuple.get<TUPLE_INDEX>() = *head_iter_++;
      super::populate_tuple (vtuple);
    }
  }
};

template <class ValueTypeTuple, 
          unsigned int INDEX>
struct Transposer <tuples::null_type, ValueTypeTuple, INDEX>
{
protected:

  void populate_tuple(ValueTypeTuple &) {}
  Transposer (tuples::null_type const &) {}
};

template <class TupleOfVectors>
typename Transposer<TupleOfVectors>::Transpose
transpose (TupleOfVectors const & tupleofv)
{
  return Transposer<TupleOfVectors>(tupleofv).get_transpose();
}

int main (void)
{
  typedef std::vector<int> Vint;
  typedef std::list<float> Lfloat;
  typedef std::vector<long> Vlong;
  
  typedef tuples::tuple<Vint, Lfloat, Vlong> TupleOfV;
  typedef Transposer<TupleOfV>::TransposeTuple TransposeTuple;

  Vint vint;
  Lfloat lfloat;
  Vlong vlong;

  srand(time(0));
  
  std::generate_n(std::back_inserter(vint), 10, bind(std::modulus<int>(), bind(rand), 15));
  std::generate_n(std::back_inserter(lfloat), 10, bind(std::divides<float>(), bind(rand), bind(rand)));
  std::generate_n(std::back_inserter(vlong), 10, bind(std::modulus<long>(), bind(rand), 15));

  Transposer<TupleOfV>::Transpose tran = transpose(make_tuple(cref(vint), cref(lfloat), cref(vlong)));
  std::copy(tran.begin(), tran.end(),
            std::ostream_iterator<TransposeTuple>(std::cout, "\n"));

  return 0;
}

