#include "concepts.h"
//#include <vector>
//#include <functional>

#define SUPER_TYPEDEFS(Super)                                               \
  typedef typename Super::argument_kind argument_kind;                      \
  typedef typename Super::result_kind result_kind;                          \
  typedef typename Super::argument_type argument_type;                      \
  typedef typename Super::result_type result_type;             

#define LOCAL_TYPEDEFS(L, OP)                                                   \
  typedef typename ET< L >::expression_type ParentKindExpr;                     \
  typedef typename ET< OP >::argument_kind ChildKind;                           \
  typedef typename ParentKindExpr::result_kind ParentKind;                      \
  typedef ChainExpr<ParentKindExpr, OP > ChainExpr;                             \

namespace LEESA {

template <class T1, class T2>
struct is_same
{
  enum { value = false };
};

template <class T>
struct is_same <T, T>
{
  enum { value = true };
};

struct empty {};
struct true_type {};
struct false_type {};

template <int v>
struct ValueToType
{
  typedef true_type type;
};

template <>
struct ValueToType<0>
{
  typedef false_type type;
};

template < typename T1=empty, typename T2=empty, typename T3=empty, 
           typename T4=empty, typename T5=empty > 
struct typelist;

template <typename T1, typename T2, typename T3, typename T4, typename T5> 
struct typelist
{
    typedef T1 head;
    typedef typelist <T2, T3, T4, T5> tail;
};

template <class List, class T>
struct contains 
{
  enum { value = is_same<typename List::head, T>::value ? 1 : 
                    contains<typename List::tail, T>::value }; 
  typedef typename ValueToType<value>::type type;
};

template <class T>
struct contains <typelist<empty,empty,empty,empty,empty>, T> 
{
  enum { value = 0 }; 
  typedef typename ValueToType<value>::type type;
};

template <class T>
class Array
{
  public:
    typedef T * iterator;
    typedef const T * const_iterator;

    size_t size;
    T * arr;

    Array ()
      : size (0), arr(0)
    {}

    Array (const Array & a)
    : size(a.size),
      arr (size ? new T[size] : static_cast<T *>(0)) {}

    Array & operator = (Array a)
    {
      a.swap(*this);
      return *this;
    }
    void Union(Array const & a)
    {
      T * arr = new T[this->size + a.size];
      size_t i = 0;
      for (i;i < this->size; ++i)
        arr[i] = this->arr[i];
      for (size_t j;j < a.size; ++j)
        arr[i+j] = a.arr[j];

      delete [] this->arr;
      this->arr = arr;
      this->size += a.size;
    }

    void push_back(T const & t)
    {
      T * arr = new T[this->size + 1];
      size_t i = 0;
      for (i;i < this->size; ++i)
        arr[i] = this->arr[i];
      arr[i] = t;
      delete [] this->arr;
      this->arr = arr;
      this->size++;
    }
    T * end() const
    {
      return arr ? arr + size : 0;
    }
    T * begin() const
    {
      return arr;
    }
    void swap (Array & a) throw ()
    {
      size_t temp = a.size;
      a.size = this->size;
      this->size = temp;
  
      T *t = a.arr;
      t.arr = this->arr;
      this->arr = t;
    }

    ~Array() { delete [] arr; size = 0; }
};


auto concept ElementConcept <typename T> 
//  : std::Assignable<T>, std::CopyConstructible<T>, 
//    std::DefaultConstructible<T> 
{
//  typename Children = typename T::Children;
};
/*
auto concept ParentChildConcept <typename Parent, typename Child>
{
  requires ElementConcept<Parent>;
  requires ElementConcept<Child>;
  typename Children = typename Parent::Children;
  typename ParentContainsChild = typename contains<Children, Child>::type;
  requires std::SameType<ParentContainsChild, true_type>;
};
*/

template <class T>
struct ContainerGen
{
  typedef LEESA::Array<T> type;
};

template <class Kind>
requires ElementConcept<Kind> 
class KindLit 
{
        typedef typename ContainerGen<Kind>::type Container;
	Container s_;

public:
        typedef KindLit expression_type;
	typedef Kind    result_kind;
	typedef Kind    argument_kind;
        typedef KindLit result_type;
        typedef KindLit argument_type;
	typedef typename Container::iterator iterator;
	typedef typename Container::const_iterator const_iterator;

	//explicit KindLit () {}
	//KindLit (const KindLit & k) : s_ (k.s_) {}
	//KindLit (Kind const & k) { s_.push_back(k); }	
	KindLit & operator = (KindLit const & rhs) 
	{ 
		if (this != &rhs)
		{
			s_ = rhs.s_; 
		}
		return *this;
	}	
	KindLit (Container const & s) : s_(s) { }	
	void Union(Kind const & k)
	{
		s_.push_back(k);
	}
	void Union(Container const & c)
	{
		s_.Union(c);
	}

	operator Container () const { return s_; } 
        KindLit apply (KindLit p) { KindLit k; k = p; 
          return k; } 
};


template <class T>
struct ET
{
  typedef KindLit<T> expression_type;
  typedef KindLit<T> result_type;
  typedef KindLit<T> argument_type;
  typedef T result_kind;
  typedef T argument_kind;
};


template <class T>
struct ETBase
{
  typedef typename T::expression_type expression_type;
  typedef typename T::result_type result_type;
  typedef typename T::argument_type argument_type;
  typedef typename T::result_kind result_kind;
  typedef typename T::argument_kind argument_kind;
};

template <class T>
struct ET <KindLit<T> >	
	: public ETBase<KindLit<T> > {};

template <class L, class R>
struct ChainExpr 
{
  typedef typename ET<L>::argument_kind argument_kind;
  typedef typename ET<R>::result_kind result_kind;
  typedef typename ET<L>::argument_type argument_type;
  typedef typename ET<R>::result_type result_type;
  typedef ChainExpr<L, R> expression_type;

  //BOOST_CONCEPT_ASSERT((LEESA::SameElementsConcept<typename ET<L>::result_kind, 
  //	 				             typename ET<R>::argument_kind>));

  L l_;
  R r_;
  ChainExpr (ChainExpr const & c) 
    : l_(c.l_), r_(c.r_) 
  {}
  explicit ChainExpr (L const &l, R const & r) 
    : l_(l), r_(r) {}
  result_type operator () (argument_type p) { return r_(l_(p)); }
};

template <class L, class R = L>
struct LEESAUnaryFunction 
{
  typedef typename ET<L>::argument_type argument_type;
  typedef typename ET<R>::result_type result_type;
  typedef typename ET<L>::argument_kind argument_kind;
  typedef typename ET<R>::result_kind result_kind;
};

template <class T, class X>
struct ET <ChainExpr<T, X> > 
	: public ETBase <ChainExpr<T, X> > {};

template <class L, class H>
struct GetChildrenOp : LEESA::LEESAUnaryFunction <L,H>
{
  typedef LEESA::LEESAUnaryFunction <L, H> Super;
  SUPER_TYPEDEFS(Super);
  
  typedef typename LEESA::GetChildrenOp<argument_type, result_type> Self;
  typedef typename LEESA::ChainExpr<L, Self> expression_type;

  explicit GetChildrenOp () {}

  GetChildrenOp (GetChildrenOp const &) {}
  result_type operator () (argument_type const & arg)
  {
    //BOOST_CONCEPT_ASSERT((LEESA::ParentChildConcept<argument_kind, result_kind>));

    result_type retval;
    typedef typename ContainerGen<argument_kind>::type Container;
    Container v = arg;
    for(typename Container::iterator iter = v.begin(); iter != v.end(); ++iter)
    {
      //retval.Union(iter->children<result_kind>());
    }
    return retval;
  }
};

template <class T, class U> 
struct ET <GetChildrenOp<T, U> > 
	: public ETBase <GetChildrenOp<T, U> > {};



template <class L, class R, template<class, class> class Operator>
struct ChainedOperator
{
  typedef ChainExpr<typename ET<L>::expression_type, 
		    Operator<typename ET<L>::result_type,  
                             typename ET<R>::result_type>
		   > Op;
};

template <class L, class H>
typename ChainedOperator<L,H,GetChildrenOp>::Op
operator >> (L const &l, H)
{
	typedef GetChildrenOp<typename ET<L>::result_type, 
                              typename ET<H>::result_type> OP;
	LOCAL_TYPEDEFS(L, OP);

	return ChainExpr(ParentKindExpr(l), OP());
}

template <class Para, class Expr>
typename Expr::result_type
evaluate (Para const & p, Expr &e)
{
  //BOOST_CONCEPT_ASSERT((LEESA::SameElementsConcept<typename ET<Expr>::argument_kind, 
  //	 					     typename ET<Para>::result_kind>));

  return e(p);
}

} // namespace LEESA

using namespace LEESA;

struct A;
struct B;
struct C;

struct A
{
  typedef typelist<B, C> Children;
  B * bptr;
  C * cptr;

  A (B *bp = 0, C *cp = 0) 
    : bptr(bp),
      cptr(cp)
  { }

  template <typename Child>
  //requires ParentChildConcept<A, Child>
  Child * children (); 
};

template <>
//requires ParentChildConcept<A, B>
B * A::children<B> () { 
  return bptr;
}
template <> 
//requires ParentChildConcept<A, C>
C * A::children<C> () { 
  return cptr;
}

struct B
{
  typedef typelist<C> Children;
  C * cptr;

  B (C *cp = 0) 
    : cptr(cp)
  { }

  template <typename Child>
  //requires ParentChildConcept<B, Child>
  Child * children (); 
};

template <> 
//requires ParentChildConcept<B, C>
C * B::children<C> () { 
  return cptr;
}

struct C
{
  typedef typelist<> Children;

  template <typename Child>
  //requires ParentChildConcept<C, Child>
  Child * children (); 
};

int main(void)
{
  A a;
  B * bptr = a.children<B> ();  

  KindLit<A> ka;
  //A() >> B() >> C();
  
  return 0;
}

