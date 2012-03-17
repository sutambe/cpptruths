#include <cstdio>
#include <string>
#include <algorithm>
#include <vector>
#include <ctype.h>

#ifdef WIN32
typedef unsigned char uint8_t;
typedef int int32_t;
#endif

struct print_string : std::string
{
  print_string() : std::string() {}

  print_string(const print_string & ps) : std::string(ps) {
    printf("copy-ctor\n");
  }
  print_string(print_string && ps) : std::string(std::move(ps)) {
    printf("move-ctor\n");
  }
  print_string(const char *str) : std::string(str) {
    printf("const char * ctor\n");
  }
  print_string & operator = (const print_string & ps) {
    printf("operator = (&)\n");
    std::string::operator = (ps);
    return *this;
  }
  print_string & operator = (print_string && ps) {
    printf("operator = (&&)\n");
    std::string::operator = (std::move(ps));
    return *this;
  }
  ~print_string() {
    printf("destructor\n");
  }
};

class Simple
{
public:

  // generated from c++/cli_hdr/struct_post.erb
  /*
  Simple (void);
  Simple (const uint8_t& o,
          const int32_t& l,
          const print_string& s,
          const std::vector<print_string> & v,
          const double& d,
          const bool& b,
          const char& c)
    : o_(o), l_(l), s_(s), v_(v), d_(d), b_(b), c_(c)
  { 
    printf("regular parameter constructor\n");
  }

  Simple (uint8_t&& o,
          int32_t&& l,
          print_string&& s,
          const std::vector<print_string> && v,
          double&& d,
          bool&& b,
          char&& c)
    : o_(o), l_(l), s_(std::move(s)), v_(std::move(v)), d_(d), b_(b), c_(c)
  { 
    printf("rvalue parameter constructor\n");
  }
*/
  template <typename Title, typename Authors>
  //template <typename Title>
  Simple (uint8_t o,
          int32_t l,
          Title && s,
          Authors && v,
          double d,
          bool b,
          char c)
    : o_(o), l_(l), 
      s_(std::forward<Title>(s)), 
      v_(std::forward<Authors>(v)), 
      d_(d), b_(b), c_(c)
  { 
    printf("perfect forwarding rvalue-ref parameter constructor\n");
  }

/*  
  Simple (uint8_t o,
          int32_t l,
          print_string s,
          std::vector<print_string> v,
          double d,
          bool b,
          char c)
    : o_(o), l_(l), s_(std::move(s)), v_(std::move(v)), d_(d), b_(b), c_(c)
  { 
    printf("pass-by-value parameter constructor\n");
  }
*/
  Simple& operator= (const Simple& x);
  Simple& operator= (Simple&& x);

  void o (const uint8_t& _o);
  void o (uint8_t&& _o);
  const uint8_t& o (void) const;
  uint8_t& o (void);

  void l (const int32_t& _l);
  void l (int32_t&& _l);
  const int32_t& l (void) const;
  int32_t& l (void);

  void s (const std::string& _s);
  void s (std::string&& _s);
  const print_string& s (void) const;
  std::string& s (void);

  void d (const double& _d);
  void d (double&& _d);
  const double& d (void) const;
  double& d (void);

  void b (const bool& _b);
  void b (bool&& _b);
  const bool& b (void) const;
  bool& b (void);

  void c (const char& _c);
  void c (char&& _c);
  const char& c (void) const;
  char& c (void);

  void swap (Simple& s);

private:
  uint8_t o_;
  int32_t l_;
  print_string s_;
  std::vector<print_string> v_;
  double d_;
  bool b_;
  char c_;
}; // Simple

print_string & toUpper(print_string & s)
{
    std::transform(s.begin(), s.end(), s.begin(), toupper);
    return s;
}

int main(void)
{
  print_string test("test");
  Simple s1(10, 20, toUpper(test), { "A", "B" }, 30.40, true, 'Z');
  //Simple s1(10, 20, toUpper(test), 30.40, true, 'Z');
  //print_string name = "Test";
  //Simple s2(10, 20, name, { "Z", "Y" }, 30.40, true, 'Z');
  
  //uint8_t value = 200;
  //Simple s3(value, 20, "ABCDEFGHIJKLMNOPQRSTUVWXYZ_ABCDEFGHIJKLMNOPQRSTUVWXYZ_ABCDEFGHIJKLMNOPQRSTUVWXYZ_", 30.40, true, 'Z'); // large string to ensure string is allocated.
  //print_string ps[5] {"A", "B", "C", "D", "E" };
  //std::remove(std::begin(ps), std::end(ps), "B");
}

