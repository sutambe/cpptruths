#include "concepts.h"
//#include <iostream>
#include "mympl.h"

concept IsChild <typename Parent, typename Child>
{
  typename Children = typename Parent::Children;
  typename ParentContainsChild = typename contains<Children, Child>::type;
  requires std::SameType<ParentContainsChild, true_type>;
};

struct Parent
{
  typedef typelist<int, long> Children;
  typedef int INT;

  template <typename Child>
  requires IsChild<Parent, Child> 
  //requires std::SameType<Parent::INT, Child> 
  int children () 
  { 
    //return 1/contains<Children, Child>::value;  
    //return 1/contains<typelist<int, long>, Child>::value;  
    return is_same<Parent::INT,Child>::value;
  }
};

template <class T>
concept_map IsChild <Parent, T>
{
  //typedef Parent::Children Children;
  //typedef T Child;
};

int main(void)
{
  int i = contains<Parent::Children, int>::value;
  int j = contains<Parent::Children, float>::value;
  
  Parent p;
  return i + j + p.children<float> ();
}
