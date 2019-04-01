#include <variant>
#include <iostream>
#include <functional>
#include <tuple>
#include <any>

class Thunk {
  int acc_;
  int n_;
  std::any (*fptr_)(int, int);
public:
  Thunk(int acc, int n, std::any (*fptr)(int, int)) 
    : acc_(acc), n_(n), fptr_(fptr) {}
  std::any operator()() const {
    return (*fptr_)(acc_, n_);  
  }
};

auto trampoline = [](auto fn) {
  return [fn](auto arg1, auto arg2) {
    std::any result = fn(arg1, arg2);
    while(result.type() != typeid(int)) {
      result = std::any_cast<Thunk>(result)();
    }
    return result;
  };  
};

std::any adder(int acc, int n) {
  return n? std::any(Thunk(acc+n, n-1, adder)) : std::any(acc);
}

std::any add(int n) {
  return trampoline(adder)(0, n); 
}

template <class Func>
class thunk;

template <class R, class... Args>
class thunk<R (*)(Args...)> {
  public:
    using Variant = std::variant<R, thunk>;

    thunk(Variant(*fptr)(Args...), Args... args)
      : fptr_(fptr),
        args_(args...) {}

    Variant operator()() const {
      return std::apply(fptr_, std::move(args_));
    }

    Variant trampoline(Args... args) const {
      Variant result = (*fptr_)(std::forward<Args>(args)...);
      while(result.index()) {
        result = std::get<1>(result)();
      }
      return std::get<0>(result);
    }
  private:
    Variant(*fptr_)(Args...);
    std::tuple<Args...> args_;
};

template <class R, class... Args>
thunk(R(*)(Args...), Args...) -> thunk<R (*)(Args...)>;

template <class R, class... Args>
thunk(std::variant<R, thunk<R(*)(Args...)>>, Args...) -> thunk<R (*)(Args...)>;

thunk<int (*)(int, int)>::Variant add2(int acc, int n) {
  if(n) 
    return thunk<int (*)(int, int)>::Variant{add2, acc, n};
  else 
    return acc;
}

int main(void) {
  {
    thunk<int (*)(int, int)> t{add2};
    auto result = t.trampoline(0, 5); 
    std::cout << std::get<0>(result) << "\n";
  }

  {
    std::any result = add(5);
    std::cout << std::any_cast<int>(result) << "\n";
  }
} 
