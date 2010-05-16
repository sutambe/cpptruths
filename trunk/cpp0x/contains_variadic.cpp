#include <type_traits>

template <typename What, typename ...L> 
struct contains : std::false_type {}; 

template <typename What, typename ...L> 
struct contains <What, What, L...> : std::true_type {}; 

template <typename What, typename L1, typename ...L> 
struct contains <What, L1, L...> : contains <What, L...> {};

/*
template <typename Vector, typename What> struct contains;

template <typename What, typename Head, typename... Tail>
struct contains < vector<Head, Tail...>, What> : 
  std::conditional < std::is_same<Head, What>::value,
                     std::true_type,
                     contains < vector<Tail...>, What> >::type
{
  typedef contains type;
};

template <typename What>
struct contains <vector<>, What>
{
  typedef contains type;
  enum { value = 0 };
};
*/

/*
struct base_one { enum { value = 1 }; };
struct derived_zero : base_one { enum { value = 0 }; };

template< typename A, typename B >
struct type_equal {
   typedef derived_zero type;
};

template< typename A >
struct type_equal< A, A > {
   typedef base_one type;
};

template< typename Key, typename... Types >
struct pack_any {
   enum { value =
          std::common_type< typename type_equal< Key, Types >::type ... >::type::value 
        };
};

template <typename Vector, typename What> struct contains;

template <typename What, typename... Args>
struct contains < vector<Args...>, What>
{
  typedef contains type;
  enum { value = pack_any< What, Args...>::value };
};
*/

int main(void)
{
  static_assert(contains<int, long, int, float>::value, "'int' does not contain");
  static_assert(contains<short, long, double, float>::value, "'short'does not contain");
}

