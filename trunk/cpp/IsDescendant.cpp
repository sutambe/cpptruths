#include <iostream>
#include "mympl.h"

template <class StartVector, class Target, class Customizer>
struct IsDescendantVector;

template <class Parent, class Descendant, class Customizer>
struct IsDescendantKind
{
  typedef IsDescendantKind type;
  typedef typename ChildrenKinds<Customizer, Parent>::type ChildrenKinds;
  
  enum { value = contains <ChildrenKinds, Descendant>::value ||
                 IsDescendantVector<ChildrenKinds, Descendant, Customizer> >::value;
};

template <class Target, class Customizer>
struct IsDescendantVector<EmptyMPLVector, Target, Customizer> 
{
  typedef IsDescendantVector type;
  enum { value = 0 };
};

template <class Target, class Customizer>
struct IsDescendantVector<EmptyMPLVectorB, Target, Customizer> 
{
  typedef IsDescendantVector type;
  enum { value = 0 };
};

template <class StartVector, class Target, class Customizer>
struct IsDescendantVector
{
  typedef IsDescendantVector type;
  typedef typename front<StartVector>::type Head;
  typedef typename pop_front<StartVector>::type Tail;
  enum { value = IsDescendantKind<Head, Target, Customizer> ||
                 IsDescendantVector<Tail, Target, Customizer> >::value;
};


