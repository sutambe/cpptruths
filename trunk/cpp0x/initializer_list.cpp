#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

void function(std::vector<std::string> vec)
{
}

void function(std::initializer_list<std::string> && ilist)
{
  std::copy(ilist.begin(), ilist.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
}

class B {};
class D1 : public B {};
class D2 : public B {};
class D3 : public B {};

template <class D>
struct WrapperBase 
{
  template <class X>
  WrapperBase(const X & x)
  : d_ (x.delegate())
  { }

  WrapperBase(D & d) : d_(&d) {}

  D * delegate() const { return d_; }

  D * d_;
};

template <class D>
struct WrapperDerived1 : public WrapperBase<D> 
{
  WrapperDerived1(D & d) : WrapperBase<D>(d) {}
};

template <class D>
struct WrapperDerived2 : public WrapperBase<D> 
{
  WrapperDerived2(D & d) : WrapperBase<D>(d) {}
};


int main()
{
  std::vector<std::string> vec {"hello", "world", "test"};
  function( {"hello", "world", "test"} );
  D1 d1;
  D2 d2;
  D3 d3;
  std::vector<B> vb { d1, d2, d3 };

  WrapperDerived1<D1> wd1(d1);
  WrapperDerived2<D2> wd2(d2);
  WrapperBase<B> wbb(wd1);
  std::vector<WrapperBase<B>> vwb { wd1, wd2 };
  std::initializer_list<WrapperBase<B>> initlist = {wd1, wd2};
}

