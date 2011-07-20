#include <cstdio>

#ifdef CONSTEXPR

constexpr bool is_prime_recursive(size_t number, size_t c)
{
  return (c*c > number) ? true : (number % c == 0) ? false : is_prime_recursive(number, c+1);
}

constexpr bool is_prime_func(size_t number)
{
  return (number <= 1) ? false : is_prime_recursive(number, 2);
}

#else

struct false_type {
  typedef false_type type;
  enum { value = 0 };
};

struct true_type {
  typedef true_type type;
  enum { value = 1 };
};

template<bool condition, class T, class U>
struct if_
{
  typedef U type;
};

template <class T, class U>
struct if_<true, T, U>
{
  typedef T type;
};

template<size_t N, size_t c> 
struct is_prime_impl
{ 
  typedef typename if_<(c*c > N),
                       true_type,
                       typename if_<(N % c == 0),
                                    false_type,
                                    is_prime_impl<N, c+1> >::type >::type type;
  enum { value = type::value };
};

template<size_t N> 
struct is_prime
{
  enum { value = is_prime_impl<N, 2>::type::value };
};

template <>
struct is_prime<0>
{
  enum { value = 0 };
};

template <>
struct is_prime<1>
{
  enum { value = 0 };
};

#endif

static bool IsPrime(size_t number) 
{    
  if (number <= 1) 
    return false;

  for (size_t i = 2; i*i <= number; ++i)        
    if (number % i == 0) 
      return false;    
  
  return true;
}

#ifdef CONSTEXPR
  #define PRIME(N) printf("IsPrime(%d) = %d,\t is_prime_func(%d) = %d\n", \
                          N, IsPrime(N), N, is_prime_func(N));
#else 
  #define PRIME(N) printf("IsPrime(%d) = %d,\t is_prime<%d>::value = %d\n", \
                          N, IsPrime(N), N, is_prime<N>::value);
#endif

#ifndef NUMBER
  #define NUMBER 2750159 
#endif

int main(void)
{
  PRIME(NUMBER);
  return 0;
}

