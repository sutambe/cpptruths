#include <variant>
#include <iostream>
#include <functional>
#include <any>

//using Result = std::variant<bool, std::function<int (int,int)>>;
using Result = std::any;

auto trampoline = [](auto fn) {
  return [fn](auto arg1, auto arg2) {
    Result result = fn(arg1, arg2);
    while(result.type() != typeid(int)) {
      result = std::any_cast<int (*)()>(result)();
    }
    return result;
  };
};


