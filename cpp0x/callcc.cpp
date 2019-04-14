#include <iostream>
#include <functional>

#ifdef VERSION1

template <typename P>
struct CPS {
  P p;

  template <typename R>
  R operator()(std::function<R(P)> access) const {
    return access(p);
  }
};

template <typename P, typename Q>
struct CALLCC {
  using P2CPSQ = std::function<CPS<Q>(P)>;
  P2CPSQ value;
  CPS<P> operator()(std::function<CPS<P>(P2CPSQ)> access) const {
    return access(value);
  }
};

int main(void) {
  CPS<int> x{10};
  auto rest = [=](int x){ return CPS<int>{x*x}; };
  CALLCC<int, int> callcc1{rest};
  auto cev = callcc1([x](auto rest) { 
      return x.operator()<CPS<int>>([rest](int value) {
        return rest(value); 
      });
  });
  cev.operator()<void>([](int value){
    std::cout << "v1: " << value << std::endl;    
  });
  
  return 0;
}

#elif VERSION2

template <typename P>
struct CPS {
  P p;

  template <typename Access>
  auto operator()(Access access) const {
    return access(p);
  }
};

template <typename P, typename Q>
struct CALLCC {
  using P2CPSQ = std::function<CPS<Q>(P)>;
  P2CPSQ value;
  CPS<P> operator()(std::function<CPS<P>(P2CPSQ)> access) const {
    return access(value);
  }
};

int main(void) {
  CPS<int> x{10};
  auto rest = [=](int x){ return CPS<int>{x*x}; };
  CALLCC<int, int> callcc1{rest};
  
  auto cev = callcc1([x](auto rest) { 
      return x([rest](int value) {
        return rest(value); 
      });
  });
  
  cev([](int value){
    std::cout << "v2: " << value << std::endl;    
  });
  
  return 0;
}

#elif VERSION3

template <typename CC>
struct CALLCC {
  CC cc;
  template <typename Access> 
  auto operator()(Access access) const {
    return access(cc);
  }
};

template <typename F>
CALLCC(F) -> CALLCC<F>;

void cps_style_square(void) {
  CALLCC<int> x_cps{10};
  x_cps([](int value){
    printf("x: %d\n", value);    
  });

  auto square_program = [=](int x){ return CALLCC<int>{x*x}; };
  CALLCC callcc1{square_program};
  
  auto x_squared_cps = callcc1([x_cps](auto rest_of_the_program) { 
      return x_cps([rest_of_the_program](int value) {
        return rest_of_the_program(value); 
      }); 
  }); 
  
  x_squared_cps([](int value){
    printf("x^2: %d\n", value);    
  }); 
}

void obvious_square() {
  int x{10};
  printf("x: %d\n", x);    
  int x_squared = x*x;
  printf("x^2: %d\n", x_squared);    
}

int main(void) {
  obvious_square();
  cps_style_square();
}

#endif

