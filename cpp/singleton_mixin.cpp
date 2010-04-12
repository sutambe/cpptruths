#include <memory>
#include <iostream>

using namespace std;

template<class SingletonClass>
class Singleton : public SingletonClass
 {
  public:
    /** Singleton creation function */
    static Singleton & instance()
    {
      if(_instance.get() == NULL)
      {
        _instance = auto_ptr<Singleton<SingletonClass> >(new Singleton);
        assert(_instance.get() != NULL);
      }
      return *_instance;
    }


  protected:
    /** Singleton instance holder */
    static auto_ptr<Singleton<SingletonClass> > _instance;
 };


 /** static instance definition */


 template<class SingletonClass>
     auto_ptr<Singleton<SingletonClass> > Singleton<SingletonClass>::_instance;

 class MyClass
 {
  public:
    virtual void bar() {};
    virtual ~MyClass() {};
  protected:
    MyClass() {};
 };

 int main (void)
 {
   Singleton<MyClass>::instance().bar();
   char c;
   std::cin >> c;
   return 0;
 }
