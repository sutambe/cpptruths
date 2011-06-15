// http://www.music.mcgill.ca/~sinclair/content/blog

#include <memory>
#include <cstdio>
#include <tuple>

using std::get;

int main (void) 
{
  auto account = [] (int b) {
    std::shared_ptr<int> balance(new int(b));
    return std::make_tuple([=] (int n) { *balance = n; },
                           [=] ()      { return *balance; });
  };

  auto a1 = account(500);
  printf("Balance: %d\n", get<1>(a1)());
  printf("Set balance to 5.\n");
  get<0>(a1)(5);
  printf("Balance: %d\n", get<1>(a1)());
  return 0;
}
