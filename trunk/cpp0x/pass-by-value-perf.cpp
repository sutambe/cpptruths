#include <string>
#include <vector>
#include <chrono>
#include <iostream>
#include <map>
	
//  10 to 20 % difference in performance depending upon
//  the platform.

using namespace std;

#ifdef PERFECT_FORWARD

template <class H0, class H1, class H2, class H3>
H0&& get_0(H0&& h0, H1&& h1, H2&& h2, H3&& h3)
{
  return move(h0);
}

template <class H0, class H1, class H2, class H3>
H1&& get_1(H0&& h0, H1&& h1, H2&& h2, H3&& h3)
{
  return move(h1);
}

template <class H0, class H1, class H2, class H3>
H2&& get_2(H0&& h0, H1&& h1, H2&& h2, H3&& h3)
{
  return move(h2);
}

template <class H0, class H1, class H2, class H3>
H3&& get_3(H0&& h0, H1&& h1, H2&& h2, H3&& h3)
{
  return move(h3);
}

#endif

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
typedef unsigned int PageNum;
class Book {
  string _name;
  vector<string> _authors;
  string _publisher;
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
#elif PERFECT_FORWARD_VARIADIC
  template <class... Args>
  Book(Args&&... args)
  :   _name     (get_0(std::forward<Args>(args)...)),
      _authors  (get_1(std::forward<Args>(args)...)),
      _publisher(get_2(std::forward<Args>(args)...)),
      _contents (get_3(std::forward<Args>(args)...))
  {} 
#elif PERFECT_FORWARD
  template <class H0, class H1, class H2, class H3>
  Book(H0&& h0, H1&& h1, H2&& h2, H3&& h3)
  :   _name     (std::forward<H0>(h0)),
      _authors  (std::forward<H1>(h1)),
      _publisher(std::forward<H2>(h2)),
      _contents (std::forward<H3>(h3))
  {}  
#else
  Book(const string & n, 
       const vector<string> & auth,
       const string & pub,
       const map<string, PageNum> & contents)
    : _name(n),
      _authors(auth),
      _publisher(pub),
      _contents(contents)
   {}
   
  Book(string && n, 
       const vector<string> & auth,
       const string & pub,
       const map<string, PageNum> & contents)
    : _name(move(n)),
      _authors(auth),
      _publisher(pub),
      _contents(contents)
   {}

  Book(const string & n, 
       vector<string> && auth,
       const string & pub,
       const map<string, PageNum> & contents)
    : _name(n),
      _authors(move(auth)),
      _publisher(pub),
      _contents(contents)
   {}

  Book(string && n, 
       vector<string> && auth,
       const string & pub,
       const map<string, PageNum> & contents)
    : _name(move(n)),
      _authors(move(auth)),
      _publisher(pub),
      _contents(contents)
   {}

  Book(const string & n, 
       const vector<string> & auth,
       string && pub,
       const map<string, PageNum> & contents)
    : _name(n),
      _authors(auth),
      _publisher(move(pub)),
      _contents(contents)
   {}
   
  Book(string && n, 
       const vector<string> & auth,
       string && pub,
       const map<string, PageNum> & contents)
    : _name(move(n)),
      _authors(auth),
      _publisher(move(pub)),
      _contents(contents)
   {}

  Book(const string & n, 
       vector<string> && auth,
       string && pub,
       const map<string, PageNum> & contents)
    : _name(n),
      _authors(move(auth)),
      _publisher(move(pub)),
      _contents(contents)
   {}

  Book(string && n, 
       vector<string> && auth,
       string && pub,
       const map<string, PageNum> & contents)
    : _name(move(n)),
      _authors(move(auth)),
      _publisher(move(pub)),
      _contents(contents)
   {}

  Book(const string & n, 
       const vector<string> & auth,
       const string & pub,
       map<string, PageNum> && contents)
    : _name(n),
      _authors(auth),
      _publisher(pub),
      _contents(move(contents))
   {}
   
  Book(string && n, 
       const vector<string> & auth,
       const string & pub,
       map<string, PageNum> && contents)
    : _name(move(n)),
      _authors(auth),
      _publisher(pub),
      _contents(move(contents))
   {}

  Book(const string & n, 
       vector<string> && auth,
       const string & pub,
       map<string, PageNum> && contents)
    : _name(n),
      _authors(move(auth)),
      _publisher(pub),
      _contents(move(contents))
   {}

  Book(string && n, 
       vector<string> && auth,
       const string & pub,
       map<string, PageNum> && contents)
    : _name(move(n)),
      _authors(move(auth)),
      _publisher(pub),
      _contents(move(contents))
   {}

  Book(const string & n, 
       const vector<string> & auth,
       string && pub,
       map<string, PageNum> && contents)
    : _name(n),
      _authors(auth),
      _publisher(move(pub)),
      _contents(move(contents))
   {}
   
  Book(string && n, 
       const vector<string> & auth,
       string && pub,
       map<string, PageNum> && contents)
    : _name(move(n)),
      _authors(auth),
      _publisher(move(pub)),
      _contents(move(contents))
   {}

  Book(const string & n, 
       vector<string> && auth,
       string && pub,
       map<string, PageNum> && contents)
    : _name(n),
      _authors(move(auth)),
      _publisher(move(pub)),
      _contents(move(contents))
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

const char * const lstr = "THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. THIS IS A A VERY LARGE STRING. ";

const char * const digits = "0123456789";

map<string, PageNum> create_map()
{
  map<std::string, PageNum> contents;
  contents[digits] = 1;
  contents[digits] = 25;
  return contents;
}

vector<string> create_vector()
{
  vector<string> v;
  v.push_back(digits);
  v.push_back(digits);
  return v;
}

int string_perf(char * argv[]);
void book_perf(char * argv[])
{
  typedef chrono::high_resolution_clock Clock;
  typedef chrono::duration<double, milli> ms;
  vector<Book> v;
  const unsigned N = atoi(argv[1]);
  v.reserve(N);
  
  Book bb("Book", { "Author1", "Author2" }, "Publisher", create_map());

  const char book_name[10] = "book_name";
  const char publisher[10]= "publisher";
  auto t0 = Clock::now();
  
  for (unsigned i = 0; i < N; ++i)
     v.emplace_back(string(book_name, book_name+10), 
                    create_vector(), 
                    string(publisher, publisher+10),  
                    create_map());
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

