#include <iostream>
#include "boost/bind.hpp"
#include "boost/utility/result_of.hpp"
#include <vector>
#include <algorithm>

template <class Functor, class Arg>
class ScopedGuard
{
    public:
      explicit ScopedGuard (Functor func, Arg arg)
        : engaged_ (false),
          func_ (func),
          arg_ (arg)
        {

        }
      ~ScopedGuard ()
      {
        if (engaged_)
          func_(arg_);
      }
      
      void release () 
      {
        engaged_ = false;
      }
      
      void activate () 
      {
        engaged_ = true;
      }
    
    private:
      bool engaged_;
      Functor func_;
      Arg arg_;
};

class DB
{
  public:
      void close (int) 
      {
        std::cerr << "DB::close called\n";
      }
};

int main (void)
{

  DB db;
  std::vector <DB *> vdb;
  vdb.push_back (&db);
  std::for_each (vdb.begin (), vdb.end(), boost::bind (&DB::close, _1, 0));
  typedef boost::result_of <bind_ (void (DB::*)(int), DB*, int)>::type TYPE;
  return 0;
}

