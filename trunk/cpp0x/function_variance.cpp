#include <functional>
#include <iostream>
#include <string>

struct Animal { 
  virtual std::string name() = 0;    
};
struct Mammal : Animal {};
struct Giraffe : Mammal { 
  std::string name() { return "Giraffe"; }    
};
struct Tiger : Mammal {
  std::string name() { return "Tiger"; }    
};

template <class T>
using Action = std::function<void (T *)>;

template <class T>
using Meta = std::function<void (Action<T>)>;

int main(void)
{
  Action<Animal> action1 = [](Animal *a) { std::cout << a->name() << "\n"; };
  Action<Giraffe> action2 = action1;
  action2(new Giraffe());
  
  Meta<Mammal> meta1 = [](Action<Mammal> action) { action(new Giraffe()); };
  Meta<Animal> meta2 = meta1;
  meta2(action1);
  
}
