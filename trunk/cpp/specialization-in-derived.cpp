#include <iostream>

struct State { };
struct FinalState { };
/*********************************************
struct Base
{
    template <class T>
    class Kind
    {
      public:
        typedef T type;
    };
};

struct Derived : public Base 
{ 
  int i;
  // Does not work 
  // template <>
  // struct Kind<FinalState> {
  //   typedef State type;
  // };
};

template <>
class Derived::Kind<FinalState> {
  public:
    typedef State type;
};
***********************************************/

struct Base {};
struct Derived : Base {};

// Original definition of Kind Will yield an error if Kind is not used properly
template<typename WhatToDo, typename T>
struct Kind;

// definition of Kind for Base selector
template<typename T>
struct Kind<Base, T>
{
  typedef T type;
};

// Here is the inheritance you wanted
template<typename T>
struct Kind<Derived, T> : Kind<Base, T>
{
};

// ... and the specialization for State
template<>
struct Kind<Derived, State>
{
  typedef FinalState type;
};

int main(void)
{
  //typedef Base::Kind<State>::type R;
  //typedef Base::Kind<FinalState>::type S;
  //typedef Derived::Kind<State>::type Rd;
  //typedef Derived::Kind<FinalState>::type Sd;
  
  typedef Kind<Base,State>::type R;
  typedef Kind<Base,FinalState>::type S;
  typedef Kind<Derived,State>::type Rd;
  typedef Kind<Derived,FinalState>::type Sd;

  std::cout << typeid(R).name() << std::endl;
  std::cout << typeid(S).name() << std::endl;
  std::cout << typeid(Rd).name() << std::endl;
  std::cout << typeid(Sd).name() << std::endl;

  return 0;
}
