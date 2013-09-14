#ifndef IN_HXX
#define IN_HXX

#include <utility>
#include <type_traits>

template <typename T> struct in;
template <typename T> struct is_in { 
  static const bool value = false;
};
template <typename T> struct is_in<in<T>> { 
  static const bool value = true;
};

template <typename T> struct in
{
  in (const T& l): v_ (l), rv_ (false) {}
  in (T&& r): v_ (r), rv_ (true) {}

  // Support for implicit conversion via perfect forwarding.
  //
  struct storage
  {
    storage (): created (false) {}
    ~storage () {
      if (created) reinterpret_cast<T*> (&data)->~T ();
    }

    bool created;
    typename std::aligned_storage<sizeof(T), alignof(T)>::type data;
  };

  template <typename T1,
            typename std::enable_if<
              std::is_convertible<T1, T>::value &&
              !is_in<typename std::remove_reference<T1>::type>::value,
              int>::type = 0>
  in (T1&& x, storage&& s = storage ())
      : v_ (*new (&s.data) T (x)), 
        rv_ (true) 
  {s.created = true;}

  in (T& l): v_ (l), rv_ (false) {} // For T1&& becoming T1&.

  // Accessors.
  //
  bool lvalue () const {return !rv_;}
  bool rvalue () const {return rv_;}

  operator const T& () const {return v_;}
  const T& get () const {return v_;}
  T&& rget () const {return std::move (const_cast<T&> (v_));}

  // Return a copy if lvalue.
  //
  T move () const
  {
    // It would have been more natural to use ?: operator, however,
    // it doesn't work as one would expecte in all the compilers.
    // For example, Clang 3.0 will always call a copy constructor.
    //
    if (rv_)
      return rget ();
    else
      return v_;
  }

private:
  const T& v_;
  bool rv_;
};

#endif // IN_HXX
