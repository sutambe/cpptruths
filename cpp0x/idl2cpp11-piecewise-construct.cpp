#include <utility>
#include <string>
#include <tuple>

template<unsigned...> struct index_tuple{};
 
template<unsigned I, typename IndexTuple, typename... Types>
struct make_indices_impl;
 
template<unsigned I, unsigned... Indices, typename T, typename... Types>
struct make_indices_impl<I, index_tuple<Indices...>, T, Types...>
{
    typedef typename
          make_indices_impl<I + 1,
                            index_tuple<Indices..., I>,
                                                  Types...>::type type;
};
 
template<unsigned I, unsigned... Indices>
struct make_indices_impl<I, index_tuple<Indices...> >
{
    typedef index_tuple<Indices...> type;
};
 
template<typename... Types>
struct make_indices
  : make_indices_impl<0, index_tuple<>, Types...>
{};

class Variable final
{
public:

  // generated from c++/cli_hdr/struct_post.erb
  Variable (void) = default;
  ~Variable (void) = default;
  Variable (const Variable&) = default;
  Variable (Variable&&) = default;

  // For each member in this class, a separate variadic typelist is needed.
  template<typename... Arg1>
  explicit Variable(std::piecewise_construct_t,
                    std::tuple<Arg1...>&& tuple1)
    : Variable(std::move(tuple1), 
               typename make_indices<Arg1...>::type())
  {}
           
private:
  template<typename... Arg1,
           unsigned... Indices1>
  explicit Variable(std::tuple<Arg1...>&& tuple1,
                    index_tuple<Indices1...>)
     : name_(std::forward<Arg1>(std::get<Indices1>(std::move(tuple1)))...)
  {}

public:

  explicit inline Variable (std::string name)
    : name_(std::move(name))
  {}

  inline Variable& operator= (const Variable& x);
  inline Variable& operator= (Variable&& x);

  inline void name (const std::string& _name);
  inline void name (std::string&& _name);
  inline const std::string& name (void) const;
  inline std::string& name (void);

  inline void swap (Variable& s);

private:
  std::string name_;
};// Variable

int main(void)
{
  char remedy[7] = { "Remedy" };

  // Consider if I must use begin and end pointers of the above array.

  // Without piecewise constructor = construct + move
  Variable v1(std::string(remedy, remedy+7));

  // With piecewise constructor = construct only
  Variable v2(std::piecewise_construct, 
              std::forward_as_tuple(remedy, remedy+7));
}

