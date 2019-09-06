// clang-tidy use_after_move.cpp -checks=bugprone-use-after-move
#include <string>
#include <vector>
#include <set>
#include <iostream>
#include <algorithm>
#include <iomanip>

using namespace std::string_literals;

void simple_use_after_move(void) {
  std::string str = "Hello, world!\n";
  std::vector<std::string> messages;
  messages.emplace_back(std::move(str));
  std::cout << str;
}

template <class Container>
bool contains(const Container& haystack, typename Container::value_type needle) {
  return std::find(haystack.begin(), haystack.end(), needle) != haystack.end();  
}

struct Cat {
  static const std::set<std::string> cat_sounds;

  static bool is_sound(const std::string& sound) {
    return contains(cat_sounds, sound);
  };

  static bool is_sound(std::string&& sound) {
    return contains(cat_sounds, std::move(sound));
  };
};
const std::set<std::string> Cat::cat_sounds = {"meow", "purrr", "hiss"};

struct Dog {
  static const std::vector<std::string> dog_sounds;

  static bool is_sound(const std::string& sound) {
    return contains(dog_sounds, std::move(sound));
  }
};
const std::vector<std::string> Dog::dog_sounds = {"Grrr", "woof", "whine"};

template <typename StringLike>
bool is_pet_sound(StringLike&& sound)
{
  bool result = Cat::is_sound(std::forward<StringLike>(sound));
  result = result || Dog::is_sound(std::forward<StringLike>(sound));
  return result;
}

template <class... Animals, typename StringLike>
bool is_animal_sound(StringLike&& sound) {
  return (Animals::is_sound(std::forward<StringLike>(sound)) || ...);
}

template <typename Func, typename... Args>
void may_be_twice(Func&& func, Args&&... args)
{
  int something = 10;
  int something_else = 20;
  bool result = func(something, std::forward<Args>(args)...);
  if (result) {
    func(something_else, std::forward<Args>(args)...);
  }
}

int main(void) {
  simple_use_after_move();
  std::cout << std::boolalpha << is_pet_sound("Grrr"s) << std::endl;
  std::cout << std::boolalpha << is_animal_sound<Cat, Dog>("woof"s) << std::endl;
  may_be_twice([](int, std::string s) { std::cout << s << std::endl; return true; }, "10"s); 
  may_be_twice([](int, const std::string& s) { std::cout << s << std::endl; return true; }, "100"s); 
}

/*
namespace std {
template <class T>
T&& move(T& t) { 
  return static_cast<T&&>(t);
}
}

class X {};

void f(X&&) {} 

int main(void) {
  X x;
  f(std::move(x));
  X x2 = x;
}
*/

