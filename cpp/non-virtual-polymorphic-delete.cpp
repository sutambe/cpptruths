#include <iostream>

template <class T>
class Ptr
{
  public:
    explicit Ptr () : ptr_(0) {}
    template <class U>
    explicit Ptr (U * ptr) : ptr_(ptr) {}
    ~Ptr () { ((U*)ptr_)->~U(); operator delete ptr_; }
  private:
    T * ptr_;
};
