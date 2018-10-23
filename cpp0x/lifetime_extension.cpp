#include <iostream>
#include <string>
#include <memory>

class Person {
public:
  std::string first_;
  std::string last_;
  uint32_t age_;
  std::shared_ptr<Person> mom_;
  std::shared_ptr<Person> dad_;

  //Person &FirstName(std::string first) { first_ = std::move(first); return *this; }
  //Person &LastName(std::string last) { last_ = std::move(last); return *this; }
  //Person &Age(uint32_t age) { age_ = age; return *this; }
  
  Person & FirstName(std::string first) { 
    std::cout << "FirstName\n";
    first_ = std::move(first); 
    return *this; 
  }
  Person & LastName(std::string last) { 
    std::cout << "LastName\n";
    last_ = std::move(last); 
    return *this; 
  }
  Person & Age(uint32_t age) { 
    std::cout << "Age\n";
    age_ = age; 
    return *this; 
  }

  Person &Parents(const Person &mom, const Person &dad) { 
    mom_ = std::make_shared<Person>(mom); 
    dad_ = std::make_shared<Person>(dad); 
    return *this; 
  }

  Person(Person &&) { std::cout << "Person(Person &&)\n"; }
  Person(const Person &) { std::cout << "Person(const Person &)\n"; }
  Person() { std::cout << "Person()\n"; }
  ~Person() {
    std::cout << "~Person\n";
  }
};

int main(void) {
  //auto &[first, last, age, mom, d] = Person().FirstName("M").LastName("Z").Age(35);
  //const auto &mom = Person().FirstName("M").LastName("Z").Age(35);
  //const auto &dad = Person().FirstName("D").LastName("Z").Age(35);
  //std::cout << "Creating child\n";
  //Person child = Person().FirstName("C").LastName("Z").Age(2).Parents(mom, dad);
  Person child = Person().FirstName("C").LastName("Z").Age(2);
}

