#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/reverse_fold.hpp> 

#include <boost/tuple/tuple.hpp> 
#include <boost/tuple/tuple_io.hpp> 

using namespace boost;

template <class MPLSequence>
struct GetTupleType
{
  // Adopted from [Boost-users] mailing list question titled "mpl sequence -> tuple"
  // Answered by Steven Watanabe.

  typedef typename 
    boost::mpl::reverse_fold<MPLSequence, 
                             boost::tuples::null_type, 
                             boost::tuples::cons<boost::mpl::_2, 
                                                 boost::mpl::_1> 
                            >::type type; 
};

template <class Vector, 
          class VTuple = typename GetTupleType<Vector>::type,
          unsigned int TUPLE_INDEX = 0, 
          unsigned int VECTOR_SIZE = mpl::size<Vector>::value >
struct Transposer 
          : Transposer <typename mpl::pop_front<Vector>::type, 
                          VTuple,
                          TUPLE_INDEX + 1, 
                          VECTOR_SIZE - 1>
{
  typedef typename mpl::front<Vector>::type Head;
  typedef typename mpl::pop_front<Vector>::type Tail;
  typedef Transposer<Tail, VTuple, TUPLE_INDEX + 1, VECTOR_SIZE - 1> super;
  typedef std::vector<VTuple> Transpose;
  
  using super::add;

  void add (std::vector<Head> const & hv)
  {
    this->head_vector_ = hv;
  }

  Transpose get_transpose ()
  {
    Transpose tran;
    int i = 0;
    for(typename std::vector<Head>::const_iterator iter = head_vector_.begin();
        iter != head_vector_.end();
        ++iter, ++i)
    {
      VTuple vtuple;
      this->populate_tuple(vtuple, i);  
      tran.push_back(vtuple);
    }
    return tran;
  }

private:

  std::vector<Head> head_vector_;

protected:

  void populate_tuple(VTuple & vtuple, int i)
  {
    vtuple.get<TUPLE_INDEX>() = this->head_vector_[i];
    super::populate_tuple (vtuple, i);
  }

};

template <class Vector, class VTuple, unsigned int INDEX>
struct Transposer <Vector, VTuple, INDEX, 0>
{
  void add (void);
  void populate_tuple(VTuple &, int i) {}
};

int main (void)
{
  typedef mpl::vector<int, float, long> Numerical;
  Transposer<Numerical> vh;
  
  std::vector<int> vint;
  std::vector<float> vfloat;
  std::vector<long> vlong;

  std::generate_n(std::back_inserter(vint), 10, rand);
  std::generate_n(std::back_inserter(vfloat), 10, rand);
  std::generate_n(std::back_inserter(vlong), 10, rand);

  vh.add(vint);
  vh.add(vfloat);
  vh.add(vlong);

  Transposer<Numerical>::Transpose tran = vh.get_transpose();
  std::copy(tran.begin(), tran.end(), 
            std::ostream_iterator<GetTupleType<Numerical>::type>(std::cout, "\n"));

  return 0;
}

