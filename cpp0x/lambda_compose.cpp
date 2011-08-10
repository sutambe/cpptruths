#include <cstdio>
#include <functional>

template <typename F, typename F_ret, typename... F_args, 
          typename G, typename G_ret, typename... G_args> 
std::function<G_ret (F_args...)>
     composer(F f, F_ret (F::*)(F_args...) const , 
              G g, G_ret (G::*)(G_args...) const)
{
  // Cannot create and return a lambda. So using std::function as a lambda holder.
  std::function<G_ret (F_args...)> holder;
  holder = [f, g](F_args... args) { return g(f(args...)); }; 
  return holder;
}

template<typename F_ret, typename... F_args>
std::function<F_ret (F_args...)> 
make_function (F_ret (*f)(F_args...))
{
  return f;
}

template<typename F, typename F_ret, typename... F_args>
std::function<F_ret (F_args...)>
make_function (F_ret (F::*func)(F_args...), F & obj)
{
  std::function<F_ret (F_args...)> holder;
  holder = [func, &obj](F_args... args) { return (obj.*func)(args...); }; 
  return holder;
}

template<typename F, typename F_ret, typename... F_args>
std::function<F_ret (F_args...)>
make_function (F_ret (F::*func)(F_args...) const, F const & obj)
{
  std::function<F_ret (F_args...)> holder;
  holder = [func, &obj](F_args... args) { return (obj.*func)(args...); }; 
  return holder;
}

template<typename F, typename G>
auto operator >> (F f, G g)
  -> decltype(composer(f, &F::operator(), g, &G::operator())) 
{ 
  return composer(f, &F::operator(), g, &G::operator()); 
}

template<typename F, typename G_ret, typename... G_args>
auto operator >> (F f, G_ret (*g)(G_args...))
  -> decltype(f >> make_function(g)) 
{ 
  return f >> make_function(g); 
}

template<typename F, typename G_ret, typename... G_args>
auto operator >> (G_ret (*g)(G_args...), F f)
  -> decltype(make_function(g) >> f) 
{ 
  return make_function(g) >> f; 
}

/* // Can't overload operators for pointers.
std::function<int (int, int)> 
operator >> (int (*f)(int, int), int (*g)(int))
{
  std::function<G_ret (F_args...)> holder;
  holder = [f, g](int i, int j) { return g(f(i, j)); }; 
  return holder;
}
*/

int increment(int i)
{
  return i+1;
}

int sum(int i, int j)
{
  return i+j;
}

struct math 
{
  int increment (int i) {
    return i+1;
  }

  int sum (int i, int j) const {
    return i+j;
  }
};

int main(void)
{
  auto l1 = [](int i , int j) { return i + j; };
  auto l2 = [](int a) { return a*a; };
  
  auto l3 = l1 >> l2 >> l2 >> increment;  // does 11 allocs on Linux.
  auto l4 = sum >> l2;

  printf("%d\n", l4(3, 3));
  printf("%d\n", (make_function(sum) >> increment)(3, 3));

  math m;
  printf("%d\n", (make_function(&math::sum, m) >> make_function(&math::increment, m))(2, 3));

  return 0;
}
