#include <iostream>
#include <algorithm>
#include <type_traits>
#include <utility>
#include <memory>
#include <string>
#include <cstring>
#include <optional>

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
class FixedAny {
public:
  static_assert(StorageSize >= std::max(sizeof(std::shared_ptr<int>), sizeof(std::string)));
  static_assert(StorageSize % 16  == 0);
  
  template <class T>
  explicit FixedAny(T&& t) 
    : copy_ctor(&call_copyctor<std::remove_reference_t<T>>),
      move_ctor(&call_movector<std::remove_reference_t<T>>),
      destructor(&call_destructor<std::remove_reference_t<T>>) {
    new (&storage_) std::remove_reference_t<T>(std::forward<T>(t));
  }
  
  FixedAny(FixedAny &other)
    : copy_ctor(other.copy_ctor),
      move_ctor(other.move_ctor),
      destructor(other.destructor) {
      copy_ctor(&storage_, &other.storage_);
  }
  
  FixedAny(const FixedAny &other)
    : copy_ctor(other.copy_ctor),
      move_ctor(other.move_ctor),
      destructor(other.destructor) {
      copy_ctor(&storage_, &other.storage_);
  }
  
  FixedAny &operator=(const FixedAny &other) {
      destructor(&storage_);
      copy_ctor = other.copy_ctor;
      move_ctor = other.move_ctor;
      destructor = other.destructor,
      copy_ctor(&storage_, &other.storage_);
  }
  
  FixedAny &operator=(FixedAny &other) {
      destructor(&storage_);
      copy_ctor = other.copy_ctor;
      move_ctor = other.move_ctor;
      destructor = other.destructor,
      copy_ctor(&storage_, &other.storage_);
      return *this;
  }

  template <class T>
  FixedAny &operator=(T&& t) {
    destructor(&storage_);
    copy_ctor = &call_copyctor<std::remove_reference_t<T>>;
    move_ctor = &call_movector<std::remove_reference_t<T>>;
    destructor = &call_destructor<std::remove_reference_t<T>>;
    new (&storage_) std::remove_reference_t<T>(std::forward<T>(t));
    return *this;
  }
  
  FixedAny(FixedAny &&other) 
    : copy_ctor(other.copy_ctor),
      move_ctor(other.move_ctor),
      destructor(other.destructor) {
      move_ctor(&storage_, &other.storage_);
  }
  
  FixedAny &operator=(FixedAny &&other) {
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
  
  void swap(FixedAny &other) {
    using std::swap;
    swap(storage_, other.storage_);
    swap(destructor, other.destructor);
  }
  
  ~FixedAny() {
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

struct FixedAnyBase {};

template <unsigned int StorageSize = 32>
class FixedAny : public FixedAnyBase {
public:
  static_assert(StorageSize >= std::max(sizeof(std::shared_ptr<int>), sizeof(std::string)));
  static_assert(StorageSize % 16  == 0);

#define SMALL_ANY_ENABLE_IF_CONDITION !std::is_convertible_v<std::remove_reference_t<T>, FixedAnyBase> && sizeof(std::remove_reference_t<T>) <= StorageSize

  template <class T>
  explicit FixedAny(T&& t, std::enable_if_t<SMALL_ANY_ENABLE_IF_CONDITION> * = nullptr) 
    : copy_ctor(nullptr),
      destructor(nullptr) {
    if constexpr (std::is_array_v<std::remove_reference_t<T>>) {
      static_assert(std::is_trivially_constructible_v<std::remove_extent_t<std::remove_reference_t<T>>>);
      std::memcpy(&storage_, &t, sizeof(std::remove_reference_t<T>));
    }
    else {
      new (&storage_) std::remove_reference_t<T>(std::forward<T>(t));
      copy_ctor = &call_copyctor<std::remove_reference_t<T>>;
      destructor = &call_destructor<std::remove_reference_t<T>>;
    }
  }
  
  FixedAny(FixedAny &other)
    : copy_ctor(other.copy_ctor),
      destructor(other.destructor) {
      if(copy_ctor)
        copy_ctor(&storage_, &other.storage_);
  }
  
  FixedAny(const FixedAny &other)
    : copy_ctor(other.copy_ctor),
      destructor(other.destructor) {
      if(copy_ctor)
        copy_ctor(&storage_, &other.storage_);
  }

  template <unsigned int OtherSize>
  explicit FixedAny(const FixedAny<OtherSize>& any, std::enable_if_t<OtherSize < StorageSize>* = nullptr) 
    : copy_ctor(any.copy_ctor),
      destructor(any.destructor) {
      if(copy_ctor)
        copy_ctor(&storage_, &any.storage_);
  }
  
  FixedAny &operator=(const FixedAny &other) {
    FixedAny temp(other);
    swap(temp);
    return *this;
  }
  
  template <class T>
  std::enable_if_t<SMALL_ANY_ENABLE_IF_CONDITION, FixedAny &> 
  operator=(T&& t) {
    if(destructor) 
      destructor(&storage_);

    if constexpr (std::is_array_v<std::remove_reference_t<T>>) {
      static_assert(std::is_trivially_constructible_v<std::remove_extent_t<std::remove_reference_t<T>>>);
      std::memcpy(&storage_, &t, sizeof(std::remove_reference_t<T>));
      copy_ctor = nullptr;
      destructor = nullptr;
    }
    else {
      new (&storage_) std::remove_reference_t<T>(std::forward<T>(t));
      copy_ctor = &call_copyctor<std::remove_reference_t<T>>;
      destructor = &call_destructor<std::remove_reference_t<T>>;
    }
    return *this;
  }
  
  FixedAny(FixedAny &&other) 
    : storage_(other.storage_),
      copy_ctor(other.copy_ctor),
      destructor(other.destructor) {
      other.destructor = nullptr;
      other.copy_ctor = nullptr;
  }
  
  FixedAny &operator=(FixedAny &&other) {
      if(destructor) 
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
  
  void swap(FixedAny &other) {
    using std::swap;
    swap(storage_, other.storage_);
    swap(copy_ctor, other.copy_ctor);
    swap(destructor, other.destructor);
  }
  
  ~FixedAny() {
    if(destructor)
      destructor(&storage_);
  }
  
  typename std::aligned_storage<StorageSize>::type storage_;
  void (*copy_ctor)(void *, const void *);
  void (*destructor)(void *);
  
protected:

  template <class T>
  static void call_destructor(void *t) {
    static_cast<T *>(t)->~T();   
  }

  template <class T>
  static void call_copyctor(void *dst, const void *src) {
    new (dst) T(*static_cast<const T *>(src));
  }
#undef SMALL_ANY_ENABLE_IF_CONDITION  
};

struct trivial {
  uint8_t a;
  uint32_t b;
};

int main(void) {
  {
    char big_char_arr[33];
    trivial small_trivial_arr[4];
    trivial big_trivial_arr[5];
    std::string small_nontrivial_arr[1];
    Test t2;
    FixedAny<> any1(t2);
    FixedAny<> any2(any1);
    FixedAny<> any3(std::string("******************************************** large ******************************************"));
    FixedAny<> any4 = any3;
    FixedAny<> any6("******* small 32 bytes ********");
    FixedAny<64> bigany(any6);
    FixedAny<> any7(small_trivial_arr);
    FixedAny<> any8(t2); any8 = small_trivial_arr;
    any1 = std::make_shared<Test>();
    std::optional<FixedAny<>> optany1;
    std::optional<FixedAny<>> optany2(any7);
    std::optional<FixedAny<>> optany3 = any8;

    //FixedAny<> compiler_error1("******************************************** large ******************************************");
    //FixedAny<32> compiler_error2(bigany);
    //FixedAny<> compiler_error3(t2); compiler_error3 = big_char_arr;
    //FixedAny<> compiler_error4(big_trivial_arr);
    //FixedAny<> compiler_error5(small_nontrivial_arr);
    //FixedAny<> compiler_error6(t2); compiler_error6 = small_nontrivial_arr;
  }
  std::cout << test_count << "\n" << "sizeof(any) = " << sizeof(FixedAny<>) << "\n";
} 
