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

template<typename F, typename G>
auto operator >> (F f, G g)
  -> decltype(composer(f, &F::operator(), g, &G::operator())) 
{ 
  return composer(f, &F::operator(), g, &G::operator()); 
}
/*
template <typename F>
F lambda_return(F f)
{
  return f;
}

auto func() -> decltype(lambda_return([]{}))
{
  return lambda_return([]{});
}
*/
int main(void)
{
  auto l1 = [](int i , int j) { return i + j; };
  auto l2 = [](int a) { return a*a; };

  printf("%d\n", (l1 >> l2 >> l2 >> l2)(2, 3));

  return 0;
}
