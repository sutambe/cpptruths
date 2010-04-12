#include <tr1/memory>

class Employee 
{
  public:
    typedef std::tr1::shared_ptr<Employee> Ptr;
    virtual ~Employee () {}                    // Native support for polymorphic destruction.
    virtual Ptr create () const = 0; // Virtual constructor (creation) 
    virtual Ptr clone () const = 0;  // Virtual constructor (copying) 
};
class Manager : public Employee     // "is-a" relationship
{
  public:

    Manager () {}                     // Default constructor
    Manager (Manager const &) {}      // Copy constructor
    ~Manager () {}
    Ptr create () const       // Virtual constructor (creation) 
    {
      return Ptr(new Manager());
    }
    Ptr clone () const        // Virtual constructor (copying) 
    {
      return Ptr(new Manager (*this));
    }
};

int main(void)
{
  Employee const &e = Manager();
  Employee::Ptr e2 = e.clone();

  return 0;
}
