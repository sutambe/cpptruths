#include <iostream>
/*
class safe_bool_base {
  public:
    typedef void (safe_bool_base::*bool_type)() const;
    void this_type_does_not_support_comparisons() const {}
  protected:
 
    safe_bool_base() {}
    safe_bool_base(const safe_bool_base&) {}
    safe_bool_base& operator=(const safe_bool_base&) {return *this;}
    ~safe_bool_base() {}
  };

  // For testability without virtual function.
  template <typename T=void> 
  class safe_bool : private safe_bool_base {
  public:
    operator bool_type() const {
      return (static_cast<const T*>(this))->boolean_test()
        ? &safe_bool_base::this_type_does_not_support_comparisons : 0;
    }
  protected:
    ~safe_bool() {}
  };

 
  // For testability with a virtual function.
  template<> 
  class safe_bool<void> : private safe_bool_base {
  public:
    operator bool_type() const {
      return boolean_test() 
        ? &safe_bool_base::this_type_does_not_support_comparisons : 0;
        
      safe_bool_base::this_type_does_not_support_comparisons();
    }
  protected:
    virtual bool boolean_test() const=0;
    virtual ~safe_bool() {}
  };
 
  template <typename T> 
    bool operator==(const safe_bool<T>& lhs, bool b) {
      if (b)
      {
          if (lhs) return true;
          else return false;
      }
      else
      {
          if (lhs) return false;
          else return true;
      }
  }
 
  template <typename T> 
    bool operator==(bool b, const safe_bool<T>& rhs) {
      if (b)
      {
          if (rhs) return true;
          else return false;
      }
      else
      {
          if (rhs) return false;
          else return true;
      }
  }
 
 
  template <typename T, typename U> 
    void operator==(const safe_bool<T>& lhs,const safe_bool<U>& rhs) {
      lhs.this_type_does_not_support_comparisons();  
  }
 
  template <typename T,typename U> 
  void operator!=(const safe_bool<T>& lhs,const safe_bool<U>& rhs) {
    lhs.this_type_does_not_support_comparisons();
  }


class Testable_with_virtual : public safe_bool<> {
  public:
    virtual ~Testable_with_virtual () {}
  protected:
    virtual bool boolean_test() const {
      // Perform Boolean logic here
      return true;
    }
  };
 
 class Testable_without_virtual : 
    public safe_bool <Testable_without_virtual> //CRTP 
 {
  public:
    // NOT virtual 
    bool boolean_test() const {
      // Perform Boolean logic here
      return false;
    }
  };

int main (void)
{
  Testable_with_virtual t1, t2;
  Testable_without_virtual p1, p2;
  if (t1) {}
  if (p1 == false) 
  {
    std::cout << "p1 == false\n";
  }
  //  if (p1 == p2) {}
    if (t1 != t2) {}

  return 0;
}
*/

/*
class Testable1
{
    bool ok_;
    typedef void (Testable1::*bool_type)() const;
    void this_type_does_not_support_comparisons() const {}
  public:
    explicit Testable1(bool b=true):ok_(b) {}
        
    operator bool_type() const {
       return ok_==true ? 
       &Testable1::this_type_does_not_support_comparisons : 0;
    }
};

class Testable2
{
    bool ok_;
    typedef void (Testable2::*bool_type)() const;
    void this_type_does_not_support_comparisons() const {}
  public:
    explicit Testable2(bool b=false):ok_(b) {}
        
    operator bool_type() const {
       return ok_==true ? 
       &Testable2::this_type_does_not_support_comparisons : 0;
    }
};

int main (void)
{
  Testable1 t1, t3;
  Testable2 t2;
  if (t1) {}
  if (t2) {}
  if (t1 < 0) {}

  return 0;
}
*/
struct Testable 
{
    operator bool() const {
          return false;
            }
};
struct AnotherTestable 
{
    operator bool() const {
          return true;
            }
};
int main (void)
{
Testable a;
AnotherTestable b;
if (a == b) { /* blah blah blah*/ }
if (a < 0) { /* blah blah blah*/ }
// The above comparisons are accidental and are not intended but compile
// happily compiles them.
return 0;}
