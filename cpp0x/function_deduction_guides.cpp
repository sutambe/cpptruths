#include <functional>
#include <type_traits>
#include <iostream>

struct Test {
  void Nonconst(int) {}
  void Const(int) const {   }
  void v(int) volatile {   }
  void cv(int) const volatile {   }
  void rvalue(int) && {
    std::cout << "rvalue\n";
  }
  void rvalue_const(int) const && {
    std::cout << "rvalue_const\n";
  }
};

namespace callable {

template <class... Args>
struct PtrCallable : std::function<Args...> {
  using std::function<Args...>::function;
};

template <class... Args>
struct RefCallable : std::function<Args...> {
  using std::function<Args...>::function;
};

template<class R, class C, class... ArgTypes> 
PtrCallable(R(C::*)(ArgTypes...)) -> PtrCallable<R(C*, ArgTypes...)>;

template<class R, class C, class... ArgTypes> 
PtrCallable(R(C::*)(ArgTypes...) const) -> PtrCallable<R(const C*, ArgTypes...)>;

template<class R, class C, class... ArgTypes> 
RefCallable(R(C::*)(ArgTypes...)) -> RefCallable<R(C&, ArgTypes...)>;

template<class R, class C, class... ArgTypes> 
RefCallable(R(C::*)(ArgTypes...)const) -> RefCallable<R(const C&, ArgTypes...)>;

template<class R, class C, class... ArgTypes> 
RefCallable(R(C::*)(ArgTypes...) &&) -> RefCallable<R(C&&, ArgTypes...)>;

template<class R, class C, class... ArgTypes> 
RefCallable(R(C::*)(ArgTypes...) const &&) -> RefCallable<R(const C&&, ArgTypes...)>;
}

/*
// Works on gcc but clang does not like them in std namespace
namespace std {
template<class R, class C, class... ArgTypes> 
function(R(C::*)(ArgTypes...)) -> function<R(C*, ArgTypes...)>;

template<class R, class C, class... ArgTypes> 
function(R(C::*)(ArgTypes...) const) -> function<R(const C &, ArgTypes...)>;
}
template<class R, class C, class... ArgTypes> 
function(R(C::*)(ArgTypes...) volatile) -> function<R(volatile C*, ArgTypes...)>;

template<class R, class C, class... ArgTypes> 
function(R(C::*)(ArgTypes...) const volatile) -> function<R(const volatile C*, ArgTypes...)>;
}
*/
template<typename T, typename U, typename std::enable_if<std::is_function<T>::value, int>::type = 0> 
void test_alternate_syntax(T U::*u) {
    static_assert(std::is_same<T, void (int) const>::value);
    using PTR = void (Test::*)(int) const;
    PTR p;
    p = u;
    (void) p;
    Test t1;
    (t1.*u)(5);
}

const Test getConstTest() {
    return {};   
}

void test_function() {
   std::function<void(Test*, int)> f1 = &Test::Nonconst;
   std::function<void(const Test*, int)> f2 = &Test::Const;
   Test a;
   const Test c;
   f1(&a, 1);
   f2(&c, 1.0);
}

void test_refcallable() {
   using namespace callable;
   Test a;
   const Test c;
   RefCallable f1 = &Test::Nonconst;
   f1(a, 1);
   RefCallable f2 = &Test::Const;
   f2(a, 1);
   f2(c, 1);
   RefCallable f3 = &Test::rvalue;
   f3(Test{}, 10); 
   RefCallable f4 = &Test::rvalue_const;
   f4(getConstTest(), 10); 
}

void test_ptrcallable() {
   using namespace callable;
   Test a;
   const Test c;
   PtrCallable f1 = &Test::Nonconst;
   f1(&a, 1);
   PtrCallable f2 = &Test::Const;
   f2(&a, 1);
   f2(&c, 1);
}

int main(void) {
  test_function();
  test_refcallable();
  test_refcallable();
  test_alternate_syntax(&Test::Const);
}
