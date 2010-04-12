/** 
*  protected abstract virtual base pure virtual private destructor
*       
*  Here we go!
*/

class A;
class X
{
  virtual ~X () = 0;
  friend class A;
};

/// A protected abstract virtual base pure virtual private destructor.
X::~X () {}

class A : protected virtual X
{
};

