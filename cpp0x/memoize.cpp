#include <functional>
#include <tuple>
#include <map>
#include <unordered_map>
#include <cstdio>
#include <memory>

template <typename ReturnType, typename... Args>
std::function<ReturnType (Args...)> memoize(ReturnType (*func) (Args...))
{
    auto cache = std::make_shared<std::map<std::tuple<Args...>, ReturnType>>();
    return ([=](Args... args) mutable  {
            std::tuple<Args...> t(args...);
            if (cache->find(t) == cache->end())                
            {
                (*cache)[t] = func(args...);
            }
            else
            {
              printf("Using cache %d\n", args...);
            }
            return (*cache)[t];
    });
}

template <typename F_ret, typename... F_args>
std::function<F_ret (F_args...)> memoized_recursion(F_ret (*func)(F_args...))
{
  typedef std::function<F_ret (F_args...)> FunctionType;
  static std::unordered_map<decltype(func), FunctionType> functor_map;
  
  if(functor_map.find(func) == functor_map.end())
    functor_map[func] = memoize(func);

  return functor_map[func];
}

/*
template<unsigned long (*f)(unsigned)> 
struct StaticMemoizer
{
  typedef std::function<unsigned long (unsigned)> FUNC;
  static FUNC get() 
  {
    static FUNC func = f;
    return func;
  }
};
*/
template <typename F_ret, typename... F_args>
struct static_memoizer
{
  template <F_ret (*func)(F_args...)>
  static std::function<F_ret (F_args...)> get()
  {
    static std::function<F_ret (F_args...)> mfunc (memoize(func));
    return mfunc;
  }
};

template <typename F_ret, typename... F_args>
class ReclaimableMemoizer 
{
  typedef std::function<F_ret (F_args...)> FunctionType;
  typedef std::unordered_map<F_ret (*)(F_args...), FunctionType> FunctorMapType;
  static FunctorMapType functor_map;

public:  
  static FunctionType get_function(F_ret (*func)(F_args...))
  {
    if(functor_map.find(func) == functor_map.end())
      functor_map[func] = memoize(func);
    
    return functor_map[func];
  }
  static void clear(F_ret (*func)(F_args...))
  {
    functor_map[func] = memoize(func);
  }
};

template <typename F_ret, typename... F_args>
typename ReclaimableMemoizer<F_ret, F_args...>::FunctorMapType ReclaimableMemoizer<F_ret, F_args...>::functor_map;

template <typename F_ret, typename... F_args>
std::function<F_ret (F_args...)> reclamable_memoized_recursion(F_ret (*func)(F_args...))
{
  return ReclaimableMemoizer<F_ret, F_args...>::get_function(func);
}

template <typename F_ret, typename... F_args>
void reclaim(F_ret (*func)(F_args...))
{
  ReclaimableMemoizer<F_ret, F_args...>::clear(func);
}

//template <typename ReturnType, typename... Args, ReturnType (*func)(Args...)>
//template <typename ReturnType, typename... Args, ReturnType *r>
//template <typename... T, T*... t>
template <typename T, T* t>
void adder()
{
  
}

int add3 (int x)
{
  return x+3;
}

// memoize recursive functions.
unsigned long fibonacci(unsigned n)
{
  //return (n < 2) ? n :  reclamable_memoized_recursion(fibonacci)(n - 2) + reclamable_memoized_recursion(fibonacci)(n - 1);
  return (n < 2) ? n :  memoized_recursion(fibonacci)(n - 2) + memoized_recursion(fibonacci)(n - 1);
}

static int S;
int main(void)
{
  auto func = memoize(add3);
  printf("%d\n", func(5));
  printf("%d\n", func(5));
  printf("fibonacci(%d) = %lu\n", 7, fibonacci(7));
  printf("fibonacci(%d) = %lu\n", 8, static_memoizer<unsigned long, unsigned>::get<fibonacci>()(8));
  adder<int, &S>();

  reclaim(fibonacci);
  printf("fibonacci(%d) = %lu\n", 7, fibonacci(7));


  return 0;
}

