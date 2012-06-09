#include <string>
#include <vector>
#include <chrono>
#include <iostream>

class A
{
   std::string str_;
public:
#ifdef PASS_BY_VALUE
   explicit A(std::string str) : str_(std::move(str)) {}
#else
   explicit A(const std::string& str) : str_(str) {}
   explicit A(std::string&& str) : str_(std::move(str)) {}
#endif
};

static_assert(std::is_nothrow_move_constructible<A>::value, "");

int main(int argc, char * argv[])
{
   typedef std::chrono::high_resolution_clock Clock;
   typedef std::chrono::duration<double, std::milli> ms;
   std::vector<A> v;
   const unsigned N = atoi(argv[1]);
   v.reserve(N);
   std::string s("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");
   auto t0 = Clock::now();
   for (unsigned i = 0; i < N; ++i)
       v.push_back(A(s));
   auto t1 = Clock::now();
   std::cout << ms(t1-t0).count() << "ms\n";
}
