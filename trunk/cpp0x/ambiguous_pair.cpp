
// from Stephan Levavej's Going Native 2012 talk

#include <string>

struct Base {};
struct Derived : Base {};
struct MostDerived : Derived {};

void func(std::pair<int, int>) { }
void func(std::pair<std::string, std::string>) { }
void func(std::pair<Base *, Base *>) { }
void func(std::pair<Derived *, Derived *>) { }
void func(Base *) {}
void func(Derived *) {}

int main(void)
{
  func(std::make_pair(10,20));
  func(std::make_pair("10", "20"));

  MostDerived * md_ptr = nullptr;
  func(std::make_pair(md_ptr, md_ptr)); // ambiguous
  
  func(md_ptr); // not ambiguous

  return 0;
}

