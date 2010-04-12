#include <stdio.h>

const char * str1 = "myErr";
const char * str2 = "myErrDerived";

struct myErr {
  myErr() {
    printf("  Base default constructor, this=%p\n", this);
  }
  myErr(const myErr& other) {
    printf("  Base copy-constructor, this=%p from that=%p\n", this, &other);
  }
  virtual const char * to_string() {
    return str1;
  }
  virtual void raise() {
    throw *this;
  }
  virtual ~myErr() {
    printf("  Base destructor, this=%p\n", this);
  }
};

struct myErrDerived : public myErr {
  myErrDerived() {
    printf("  Derived default constructor, this=%p\n", this);
  }
  myErrDerived(const myErrDerived& other) 
    : myErr(other)
  {
    printf("  Derived copy-constructor, this=%p from that=%p\n", this, &other);
  }
  virtual const char * to_string() {
    return str2;
  }
  virtual void raise() {
    throw *this;
  }
  virtual ~myErrDerived() {
    printf("  Derived destructor, this=%p\n", this);
  }
};

int main() {
  try {
    try {
      myErrDerived e;
      throw e;
    } catch (myErr& err) {
      printf("A Inner catch, &err=%p, %s\n", &err,err.to_string());
      //throw;     // The original derived object is thrown.
      err.raise();   // A copy of the original derived object is thrown.
    }
  } catch (myErr& err) {
    printf("A Outer catch, &err=%p, %s\n", &err, err.to_string());
  }
  printf("---\n");
  try {
    try {
      myErrDerived e;
      throw e;
    } catch (myErr& err) {
      printf("B Inner catch, &err=%p, %s\n", &err, err.to_string());
      throw err; // derived object is lost.
    }
  } catch (myErr& err) {
    printf("B Outer catch, &err=%p, %s\n", &err,err.to_string());
  }
  return 0;
}
