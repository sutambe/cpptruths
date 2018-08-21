#include <iostream>
#include <string>

struct configs {
  std::string a;

  configs() {}
  configs(const configs& c) : a{c.a} {
    std::cout << "configs(const configs &)\n";
  }
  configs(configs&& c) : a{std::move(c.a)} {
    std::cout << "configs(configs&&)\n";
  }
  configs & operator = (const configs& c) {
    std::cout << "operator = (const configs &)\n";
    this->a = c.a;
    return *this;
  }  
  configs & operator = (configs&& c) {
    std::cout << "operator = (configs &&)\n";
    this->a = std::move(c.a);
    return *this;
  }  
  configs set_a(const std::string & a) const & {
    std::cout << "cosnt &" << std::endl;
    configs temp = *this;
    temp.a = a;
    return temp;
  }
  configs& set_a(const std::string & a) & {
    std::cout << "&" << std::endl;
    this->a = a;
    return *this;
  }
  configs set_a(const std::string & a) && {
    std::cout << "&&" << std::endl;
    this->a = a;
    return std::move(*this);
    //return *this;
  }
  configs move() const {
    return std::move(const_cast<configs &>(*this));
  }
};

std::ostream & operator << (std::ostream &o, const configs & c) {
  o << c.a;
  return o;
}

void foo(configs&&) { }

int main(void) {
  configs c1 = configs().set_a("A").set_a("AA").set_a("AAA");
  std::cout << c1 << std::endl << std::endl;

  configs c2;
  c2.set_a("A").set_a("AA").set_a("AAA");
  std::cout << c2 << std::endl << std::endl;

  const configs c3;
  const configs c4 = c3.set_a("A").set_a("AA").set_a("AAA");
  std::cout << c3 << std::endl;
  std::cout << c4 << std::endl;
  foo(c4.move());
}
