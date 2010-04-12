#include <list>
#include <iostream>


class X;


class C
{
  public:

      typedef std::list<X*> My_List;

      void f();
      void add(X * x) { mylist_.push_back(x); }

  private:

      My_List mylist_;
};


class X
{
  public:
      X(C & c) : c_(c)
                   {
                     }

      void do_something()
        {
            ++counter_;
            std::cout << this << " X::do_something (" << counter_ << ")\n";
            if (counter_ == 3)
              {
                  c_.f();
                  counter_ = 0;
                }
              }

  private:
              static int counter_;
              C & c_;
};


// static
int X::counter_(0);


template <class T>
class List_Replace_Guard
{
    List_Replace_Guard

};


void C::
f()
{
    std::cout << "Entering C::f()" << std::endl;

#if 1
    My_List local_list;
    local_list.swap(mylist_);
    List_Replace_Guard guard(local_list, mylist_);

    while (local_list.empty() == false)
      {
          std::auto_ptr<X> x(local_list.front());
          local_list.pop_front();
          x->do_something();
        }
        guard.release();

#else

        for(My_List::iterator
                    i = mylist_.begin(),
                    e = mylist_.end();
                i != e;
                ++i)
          {
              (*i)->do_something();
            }

            for(My_List::iterator
                        i = mylist_.begin(),
                        e = mylist_.end();
                    i != e;
                    ++i)
              {
                  delete *i;
                }
                mylist_.clear();
#endif

                std::cout << "Exiting C::f()" << std::endl;
}


int main()
{
    C container;
    for (int i=0; i < 10; ++i)
      {
          container.add(new X(container));
        }

        container.f();
}
 
