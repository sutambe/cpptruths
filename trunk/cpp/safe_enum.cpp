#include <algorithm>
#include <iostream>

template<typename def, typename inner = typename def::type>
class safe_enum : public def
{
  typedef typename def::type type;
  inner val;
  
  static safe_enum array[def::_end_ - def::_begin_];
  static bool init;

  static void initialize()
  {
    if(!init) // use double checked locking in case of multi-threading.
    {
      unsigned int size = def::_end_ - def::_begin_;
      for(unsigned int i = 0, j = def::_begin_; i < size; ++i, ++j)
        array[i] = static_cast<typename def::type>(j);
      init = true;
    }
  }

public:

  safe_enum(type v = def::_begin_) : val(v) {}

  inner underlying() const { return val; }
  
  static safe_enum * begin() 
  { 
    initialize(); 
    return array; 
  }

  static safe_enum * end() 
  { 
    initialize(); 
    return array + (def::_end_ - def::_begin_); 
  }

  bool operator == (const safe_enum & s) const { return this->val == s.val; }
  bool operator != (const safe_enum & s) const { return this->val != s.val; }
  bool operator <  (const safe_enum & s) const { return this->val <  s.val; }
  bool operator <= (const safe_enum & s) const { return this->val <= s.val; }
  bool operator >  (const safe_enum & s) const { return this->val >  s.val; }
  bool operator >= (const safe_enum & s) const { return this->val >= s.val; }
};

template <typename def, typename inner>
safe_enum<def, inner> safe_enum<def, inner>::array[def::_end_ - def::_begin_];

template <typename def, typename inner>
bool safe_enum<def, inner>::init = false;

struct color_def
{
  enum type
  {
      _begin_, red = _begin_, green, blue, _end_
  };
};
 
struct shape_def
{
  enum type
  {
      _begin_, circle = _begin_, triangle, square, _end_
  };
};
 
typedef safe_enum<color_def, unsigned char> color; // use int as enum holder data type (in common platforms)
// typedef safe_enum<color_def, unsigned char> color; // use uchar as enum holder
typedef safe_enum<shape_def> shape; // use uchar as enum holder

template <class Enum>
void f(Enum e)
{
  std::cout << static_cast<unsigned>(e.underlying()) << std::endl;
}

int main()
{
  color p = color::red;
  shape s = shape::triangle;
  
  std::for_each(color::begin(), color::end(), &f<color>);
  std::for_each(shape::begin(), shape::end(), &f<shape>);
/*
  //bool flag = (p >= shape::green);
  bool flag = (p <= color::blue);

  enum Color { red, green, blue };
  enum Shape { circle, square, triangle };

  Color c = red;
  bool f = (c >= triangle);
*/
}
