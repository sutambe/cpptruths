#include <string>
#include <vector>
#include <chrono>
#include <iostream>
#include <map>

//  10 to 20 % difference in performance depending upon
//  the platform.

using namespace std;

class A
{
   string str_;
public:
#ifdef PASS_BY_VALUE
   explicit A(string str) : str_(move(str)) {}
#else
   explicit A(const string& str) : str_(str) {}
   explicit A(string&& str) : str_(move(str)) {}
#endif
};

class Book {
  string _name;
  vector<string> _authors;
  string _publisher;
  typedef unsigned int PageNum;
  map<string, PageNum> _contents; 
public:
#ifdef PASS_BY_VALUE
  Book(string n, 
       vector<string> auth,
       string pub,
       map<string, PageNum> contents)
    : _name(move(n)),
      _authors(move(auth)),
      _publisher(move(pub)),
      _contents(move(contents))
   {}
#else
  Book(string & n, 
       vector<string> & auth,
       string & pub,
       map<string, PageNum> &contents)
    : _name(n),
      _authors(auth),
      _publisher(pub),
      _contents(contents)
   {}
  Book(string && n, 
       vector<string> && auth,
       string && pub,
       map<string, PageNum> && contents)
    : _name(move(n)),
      _authors(move(auth)),
      _publisher(move(pub)),
      _contents(move(contents))
   {} 
#endif  
};


static_assert(is_nothrow_move_constructible<A>::value, "");

const char * const lstr = "THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. ";


int string_perf(char * argv[]);
void book_perf(char * argv[])
{
   typedef chrono::high_resolution_clock Clock;
   typedef chrono::duration<double, milli> ms;
   vector<Book> v;
   const unsigned N = atoi(argv[1]);
   v.reserve(N);
   Book b(lstr, 
          { lstr, lstr }, 
          lstr,  
          { { "Chapter1", 1  },
            { "Chapter2", 25 } });
   auto t0 = Clock::now();
   for (unsigned i = 0; i < N; ++i)
       v.push_back(Book(b));
   auto t1 = Clock::now();
   cout << ms(t1-t0).count() << "ms\n";
}

int main(int argc, char * argv[])
{
  // string_perf(argc, argv);
  book_perf(argv);
}


int string_perf(char * argv[])
{
   typedef chrono::high_resolution_clock Clock;
   typedef chrono::duration<double, milli> ms;
   vector<A> v;
   const unsigned N = atoi(argv[1]);
   v.reserve(N);
   string s("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");
   //string s("0123456789");
   auto t0 = Clock::now();
   for (unsigned i = 0; i < N; ++i)
       v.push_back(A(s));
   auto t1 = Clock::now();
   cout << ms(t1-t0).count() << "ms\n";

   return 0;
}

