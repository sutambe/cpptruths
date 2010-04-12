#ifndef __MYMPL_H
#define __MYMPL_H

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
           typename T4=empty, typename T5=empty, typename T6=empty > 
struct typelist;

template <typename T1, typename T2, typename T3, 
          typename T4, typename T5, typename T6> 
struct typelist
{
    typedef T1 head;
    typedef typelist <T2, T3, T4, T5, T6> tail;
};

template <class List, class T>
struct contains 
{
  enum { value = is_same<typename List::head, T>::value ? 1 : 
                    contains<typename List::tail, T>::value }; 
  typedef typename ValueToType<value>::type type;
};

typedef typelist<empty,empty,empty,empty,empty,empty> EmptyMPLVector;

template <class T>
struct contains <EmptyMPLVector, T> 
{
  enum { value = 0 }; 
  typedef typename ValueToType<value>::type type;
};

//*************************** IsDescendant ***********************/

template <class StartVector, class Target>
struct IsDescendantVector;

template <class Parent, class Descendant>
struct IsDescendantKind
{
  typedef IsDescendantKind type;
  typedef typename ChildrenKinds<Parent>::type ChildrenKinds;
  
  enum { value = contains <ChildrenKinds, Descendant>::value ||
                 IsDescendantVector<ChildrenKinds, Descendant>::value };
};

template <class Target>
struct IsDescendantVector<EmptyMPLVector, Target> 
{
  typedef IsDescendantVector type;
  enum { value = 0 };
};

template <class StartVector, class Target>
struct IsDescendantVector
{
  typedef IsDescendantVector type;
  typedef typename front<StartVector>::type Head;
  typedef typename pop_front<StartVector>::type Tail;
  enum { value = IsDescendantKind<Head, Target> ||
                 IsDescendantVector<Tail, Target>::value };
};

#endif // __MYMPL_H


