#include <iostream>
#include <algorithm>
#include <type_traits>
#include <utility>
#include <memory>
#include <string>

int test_count = 0;

struct Test {
  Test() {
    ++test_count;  
  }
  Test(const Test &) {
    ++test_count;  
  }
  ~Test() {
    --test_count;
    //std::cout << "~Test\n";   
  }
};
/*
template <unsigned int StorageSize = 32>
class SmallAny {
public:
  static_assert(StorageSize >= std::max(sizeof(std::shared_ptr<int>), sizeof(std::string)));
  static_assert(StorageSize % 16  == 0);
  
  template <class T>
  explicit SmallAny(T&& t) 
    : copy_ctor(&call_copyctor<std::decay_t<T>>),
      move_ctor(&call_movector<std::decay_t<T>>),
      destructor(&call_destructor<std::decay_t<T>>) {
    new (&storage_) std::decay_t<T>(std::forward<T>(t));
  }
  
  SmallAny(SmallAny &other)
    : copy_ctor(other.copy_ctor),
      move_ctor(other.move_ctor),
      destructor(other.destructor) {
      copy_ctor(&storage_, &other.storage_);
  }
  
  SmallAny(const SmallAny &other)
    : copy_ctor(other.copy_ctor),
      move_ctor(other.move_ctor),
      destructor(other.destructor) {
      copy_ctor(&storage_, &other.storage_);
  }
  
  SmallAny &operator=(const SmallAny &other) {
      destructor(&storage_);
      copy_ctor = other.copy_ctor;
      move_ctor = other.move_ctor;
      destructor = other.destructor,
      copy_ctor(&storage_, &other.storage_);
  }
  
  SmallAny &operator=(SmallAny &other) {
      destructor(&storage_);
      copy_ctor = other.copy_ctor;
      move_ctor = other.move_ctor;
      destructor = other.destructor,
      copy_ctor(&storage_, &other.storage_);
      return *this;
  }

  template <class T>
  SmallAny &operator=(T&& t) {
    destructor(&storage_);
    copy_ctor = &call_copyctor<std::decay_t<T>>;
    move_ctor = &call_movector<std::decay_t<T>>;
    destructor = &call_destructor<std::decay_t<T>>;
    new (&storage_) std::decay_t<T>(std::forward<T>(t));
    return *this;
  }
  
  SmallAny(SmallAny &&other) 
    : copy_ctor(other.copy_ctor),
      move_ctor(other.move_ctor),
      destructor(other.destructor) {
      move_ctor(&storage_, &other.storage_);
  }
  
  SmallAny &operator=(SmallAny &&other) {
      destructor(&storage_);
      copy_ctor = other.copy_ctor;
      move_ctor = other.move_ctor;
      destructor = other.destructor,
      move_ctor(&storage_, &other.storage_);
      return *this;
  }
  
  template<class T>
  T &Get() const {
    return *static_cast<T *>(&storage_);
  }
  
  void swap(SmallAny &other) {
    using std::swap;
    swap(storage_, other.storage_);
    swap(destructor, other.destructor);
  }
  
  ~SmallAny() {
    destructor(&storage_);
  }
  
private:

  template <class T>
  static void call_destructor(void *t) {
    static_cast<T *>(t)->~T();   
  }

  template <class T>
  static void call_copyctor(void *dst, void *src) {
    new (dst) T(*static_cast<T *>(src));
  }

  template <class T>
  static void call_movector(void *dst, void *src) {
    new (dst) T(std::move(*static_cast<T *>(src)));
  }

  typename std::aligned_storage<StorageSize>::type storage_;
  void (*copy_ctor)(void *, void *);
  void (*move_ctor)(void *, void *);
  void (*destructor)(void *);
};
*/

template <unsigned int StorageSize = 32>
class SmallAny {
public:
  static_assert(StorageSize >= std::max(sizeof(std::shared_ptr<int>), sizeof(std::string)));
  static_assert(StorageSize % 16  == 0);
  
  template <class T>
  explicit SmallAny(T&& t) 
    : copy_ctor(&call_copyctor<std::decay_t<T>>),
      destructor(&call_destructor<std::decay_t<T>>) {
    new (&storage_) std::decay_t<T>(std::forward<T>(t));
  }
  
  SmallAny(SmallAny &other)
    : copy_ctor(other.copy_ctor),
      destructor(other.destructor) {
      if(copy_ctor)
        copy_ctor(&storage_, &other.storage_);
  }
  
  SmallAny(const SmallAny &other)
    : copy_ctor(other.copy_ctor),
      destructor(other.destructor) {
      if(copy_ctor)
        copy_ctor(&storage_, &other.storage_);
  }
  
  SmallAny &operator=(const SmallAny &other) {
    SmallAny temp(other);
    swap(temp);
    return *this;
  }
  
  template <class T>
  SmallAny &operator=(T&& t) {
    destructor(&storage_);
    copy_ctor = &call_copyctor<std::decay_t<T>>;
    destructor = &call_destructor<std::decay_t<T>>;
    new (&storage_) std::decay_t<T>(std::forward<T>(t));
    return *this;
  }
  
  SmallAny(SmallAny &&other) 
    : storage_(other.storage_),
      copy_ctor(other.copy_ctor),
      destructor(other.destructor) {
      other.destructor = nullptr;
      other.copy_ctor = nullptr;
  }
  
  SmallAny &operator=(SmallAny &&other) {
      destructor(&storage_);
      storage_ = other.storage_;
      copy_ctor = other.copy_ctor;
      destructor = other.destructor;
      other.copy_ctor = nullptr;
      other.destructor = nullptr;
      return *this;
  }
  
  template<class T>
  T &Get() {
    return *static_cast<T *>(&storage_);
  }
  
  template<class T>
  const T &Get() const {
    return *static_cast<T *>(&storage_);
  }
  
  void swap(SmallAny &other) {
    using std::swap;
    swap(storage_, other.storage_);
    swap(copy_ctor, other.copy_ctor);
    swap(destructor, other.destructor);
  }
  
  ~SmallAny() {
    if(destructor)
      destructor(&storage_);
  }
  
protected:

  template <class T>
  static void call_destructor(void *t) {
    static_cast<T *>(t)->~T();   
  }

  template <class T>
  static void call_copyctor(void *dst, void *src) {
    new (dst) T(*static_cast<T *>(src));
  }

  typename std::aligned_storage<StorageSize>::type storage_;
  void (*copy_ctor)(void *, void *);
  void (*destructor)(void *);
};

int main(void) {
  {
    Test t2;
    SmallAny<> any1(t2);
    SmallAny<> any2(any1);
    SmallAny<> any3(std::string("******************************************** large ******************************************"));
    SmallAny<> any4 = any3;
    SmallAny<> any5 = std::move(any4);
    any1 = std::make_shared<Test>();
  }
  std::cout << test_count << "\n" << "sizeof(any) = " << sizeof(SmallAny<>) << "\n";
} 
