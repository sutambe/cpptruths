#include <iostream>
#include <memory>
#include <set>
#include <functional>
#include <boost/shared_ptr.hpp>

#include <vector>
template<typename T>
std::shared_ptr<T> make_shared_ptr(boost::shared_ptr<T>& ptr)
{
    return std::shared_ptr<T>(ptr.get(), [ptr](T*) mutable { std::cout << "deleter\n"; /*ptr.reset();*/});
}
struct Foo
{
  ~Foo() { 
    std::cout << "~Foo\n";   
  }
};
int main()
{
  std::weak_ptr<Foo> foo_weak;
  {
    std::shared_ptr<Foo> foo_std;
    {
        boost::shared_ptr<Foo> foo_boost(new Foo);
        foo_std = make_shared_ptr(foo_boost);
        std::cout << "foo_std use count = " << foo_std.use_count() << std::endl;
        std::cout << "deleting original foo_boost\n";
    } 
    std::cout << "foo_std use count = " << foo_std.use_count() << std::endl;
    foo_weak = foo_std;
    std::cout << "deleting foo_std\n";
  } 
  std::cout << "foo_weak use count = " << foo_weak.use_count() << std::endl;
  std::cout << "deleting foo_weak\n";
}
