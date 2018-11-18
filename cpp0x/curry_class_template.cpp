#include <type_traits>
#include <functional>
#include <map>
#include <iostream>

template <template <template <class> class> class T>
struct TTT {};

template <template <class...> class C, class... T, class D = C<T...>>
std::true_type valid(std::nullptr_t);

template <template <class...> class C, class... T>
std::false_type valid(...);

template <class TrueFalse, template <class...> class C, class... ActualArgs>
struct curry_impl;

template <template <class...> class C, class... ActualArgs>
struct curry_impl<std::true_type, C, ActualArgs...> {
  using type = C<ActualArgs...>;
};

template <template <class...> class C, class... ActualArgs>
struct curry_impl<std::false_type, C, ActualArgs...> {
  template <class U>
  using apply = curry_impl<decltype(valid<C, ActualArgs..., U>(nullptr)), C, ActualArgs..., U>;
};

template <template <class...> class C>
struct curry {
  template <class U>
  using apply = curry_impl<decltype(valid<C, U>(nullptr)), C, U>;
};

int main(void) {
  //int i = curry<std::is_same>::apply<int>::apply<int>::type::value;
  curry<std::less>::apply<int>::type j;
  curry<std::map>::apply<int>::apply<long>::type k;
  std::cout << std::boolalpha << j(5000, 4000);
}
