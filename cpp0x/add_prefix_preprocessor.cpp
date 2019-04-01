// g++ -E add_prefix_preprocessor.cpp -o add_prefix_preprocessor -I ~/bin/boost_1_68_0

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/seq/for_each.hpp>

#define ADD_PREFIX(r, prefix, element) (BOOST_PP_CAT(prefix, element))
#define EXPAND(RPCS) BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_FOR_EACH(ADD_PREFIX, &NestedAsyncService::Request, RPCS))
    
struct NestedAsyncService {
  void RequestAppend() {}
  void RequestGetServerInfo() {}
};

template <class... T>
void test(T...) { 

}

int main(void) {
   //auto i = ADD_PREFIX(Append);  
   //test(EXPAND((Append)));
   test(EXPAND((Append)(GetServerInfo)));
}
