#include <string>

typedef unsigned int size_t;

class Color
{
    static size_t enum_count;
    std::string const name;
    size_t const number;

    explicit Color (const char *n) 
      : name (n), number (++enum_count) 
    {}
  public:
    operator const char * () { return name.c_str(); } 
    bool operator == (const Color &c) { return number == c.number; }
    bool operator != (const Color &c) { return ! (*this == c); }
    bool operator == (const char *c) { return name == std::string (c); }
    static Color get (size_t n)
    {
      if (n == Green.number) return Green;
      else if ( n == Red.number) return Red;
      else return Invalid;
    }
    static Color get (const char *c)
    {
      std::string cstr (c);
      if (Green.name == cstr) return Green;
      else if (Red.name == cstr) return Red;
      else return Invalid;
    }
    static Color Invalid;
    static Color Green;
    static Color Red;
};

size_t Color::enum_count = 0;

Color Color::Invalid ("Invalid");
Color Color::Green ("Green");
Color Color::Red ("Red");

int main (void)
{
  Color c = Color::Green;
  return 0;
}
