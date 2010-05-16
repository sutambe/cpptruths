#include <type_traits>

template <class... Args>
struct vector
{};

template <class T, class V>
struct push_front;

template <class T, class... Args>
struct push_front<vector<Args...>, T>
{
  typedef vector <T, Args...> type;
};

template <typename What, typename ...L> 
struct contains : std::false_type {}; 

template <typename What, typename ...L> 
struct contains <What, What, L...> : std::true_type {}; 

template <typename What, typename L1, typename ...L> 
struct contains <What, L1, L...> : contains <What, L...> {};

template <class V1, class V2>
struct intersection;

template <>
struct intersection<vector<>, vector<> > {
  typedef vector<> type;
};

template <class V2>
struct intersection<vector<>, V2> {
  typedef vector<> type;
};

template <class V1>
struct intersection<V1, vector<> > {
  typedef vector<> type;
};

template <class H1, class... Tail1, class... Args2>
struct intersection<vector<H1, Tail1...>, vector<Args2...> >
{
  typedef typename 
    intersection<vector<Tail1...>, vector<Args2...> >::type Intersection;
  typedef typename 
    std::conditional <contains<H1, Args2...>::value,
                               typename push_front<Intersection, H1>::type,
                               Intersection>::type type;
};

int main(void)
{
  
}



