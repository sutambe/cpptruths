#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/function_types/function_arity.hpp>

#include <iostream>

template<typename Func>
int select(Func f)
{
  std::cout << boost::function_types::function_arity<Func>::value
            << std::endl;
}

int main (void)
{
  select([](const std::string & s) { return s=="Sumant"; });
  
  return 0;
}
