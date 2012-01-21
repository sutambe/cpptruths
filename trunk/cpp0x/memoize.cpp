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

enum class Cache : unsigned int { NO_RECLAIM, RECLAIM };  

template <typename F_ret, typename... F_args>
std::function<F_ret (F_args...)> memoized_recursion(F_ret (*func)(F_args...), Cache c = Cache::NO_RECLAIM)
{
  typedef std::function<F_ret (F_args...)> FunctionType;
  static std::unordered_map<decltype(func), FunctionType> functor_map;
  
  if(Cache::RECLAIM == c)
    return functor_map[func] = memoize(func);
  
  if(functor_map.find(func) == functor_map.end())
    functor_map[func] = memoize(func);

  return functor_map[func];
}

template <typename Sig, Sig funcptr>
struct static_memoizer;

template <typename F_ret, typename... F_args, F_ret (*func)(F_args...)>
struct static_memoizer<F_ret (*)(F_args...), func>
{
  static std::function<F_ret (F_args...)> & get(Cache c = Cache::NO_RECLAIM)
  {
    static std::function<F_ret (F_args...)> mfunc (memoize(func));
    if(Cache::RECLAIM == c)
      mfunc = memoize(func);
    return mfunc;
  }
};

#define STATIC_MEMOIZER(func) static_memoizer<decltype(&func), &func>

template <typename ReturnType, typename... Args, ReturnType (*func)(Args...)>
//template <typename ReturnType, typename... Args, ReturnType *r>
//template <typename... T, T*... t>
//template <typename T, T* t>
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

int main(void)
{
  auto func = memoize(add3);
  printf("%d\n", func(5));
  printf("%d\n", func(5));
  printf("fibonacci(%d) = %lu\n", 7, fibonacci(7));
  memoized_recursion(fibonacci, Cache::RECLAIM);

  printf("static memoizer. fibonacci(%d) = %lu\n", 8, STATIC_MEMOIZER(fibonacci)::get()(8));
  //adder<int, int, &add3>();

  printf("fibonacci(%d) = %lu\n", 7, fibonacci(7));


  return 0;
}

