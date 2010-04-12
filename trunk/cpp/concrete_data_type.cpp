#include <new>
#include <memory>

class EventHandler 
{
  public:
    virtual ~EventHandler () {}
};
class MouseEventHandler : public EventHandler
{
  protected:
    ~MouseEventHandler () {} // A protected destructor.
  public:
     MouseEventHandler () {} // Public Constructor.
     void destroy () { delete this; }
};
class ScopedVar
{
  public:
    ScopedVar () {}
    static void * operator new (unsigned int size, void * mem);
  private:
    static void * operator new (unsigned int size);
};
int main (void)
{
  //MouseEventHandler m; // Not allowed as destructor is protected.
  EventHandler *e = new MouseEventHandler (); // Allowed
  delete e;  // This is also allowed!
  MouseEventHandler *mh = new MouseEventHandler (); // Allowed
  //delete mh;  // This is also allowed!
  mh->destroy();
  void * mem = ::operator new (sizeof (ScopedVar));
  ScopedVar * sv = new(mem) ScopedVar();
}
