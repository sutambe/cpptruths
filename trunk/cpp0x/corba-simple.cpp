#include <cstdio>
#include <string>

class Simple
{
public:

  // generated from c++/cli_hdr/struct_post.erb
  Simple (void);
  Simple (const uint8_t& o,
          const int32_t& l,
          const std::string& s,
          const double& d,
          const bool& b,
          const char& c)
    : o_(o), l_(l), s_(s), d_(d), b_(b), c_(c)
  { 
    printf("regular parameter constructor\n");
  }

  Simple (uint8_t&& o,
          int32_t&& l,
          std::string&& s,
          double&& d,
          bool&& b,
          char&& c)
    : o_(o), l_(l), s_(std::move(s)), d_(d), b_(b), c_(c)
  { 
    printf("rvalue parameter constructor\n");
  }

  /*
  Simple (uint8_t o,
          int32_t l,
          std::string s,
          double d,
          bool b,
          char c)
    : o_(o), l_(l), s_(std::move(s)), d_(d), b_(b), c_(c)
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
  const std::string& s (void) const;
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
  std::string s_;
  double d_;
  bool b_;
  char c_;
}; // Simple

int main(void)
{
  Simple s1(10, 20, "Sumant", 30.40, true, 'Z');
  std::string name = "Sumant";
  Simple s2(10, 20, name, 30.40, true, 'Z');
  uint8_t value = 200;
  Simple s3(value, 20, "Sumant", 30.40, true, 'Z');

}

