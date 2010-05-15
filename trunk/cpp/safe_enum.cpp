template<typename def, typename inner = typename def::type>
class safe_enum : public def
{
  typedef typename def::type type;
  inner val;

public:

  safe_enum(type v) : val(v) {}
  inner underlying() const { return val; }

  bool operator == (const safe_enum & s) const { return this->val == s.val; }
  bool operator != (const safe_enum & s) const { return this->val != s.val; }
  bool operator <  (const safe_enum & s) const { return this->val <  s.val; }
  bool operator <= (const safe_enum & s) const { return this->val <= s.val; }
  bool operator >  (const safe_enum & s) const { return this->val >  s.val; }
  bool operator >= (const safe_enum & s) const { return this->val >= s.val; }
};

struct color_def
{
  enum type
  {
      red, green, blue
  };
};
 
struct shape_def
{
  enum type
  {
      circle, triangle, square
  };
};
 
typedef safe_enum<color_def, unsigned char> color; // use int as enum holder data type (in common platforms)
// typedef safe_enum<color_def, unsigned char> color; // use uchar as enum holder
typedef safe_enum<shape_def> shape; // use uchar as enum holder
 
void f(color p)
{
  p = color::green;
}
 
int main()
{
  color p = color::red;
  shape s = shape::triangle;
  f(p);
  //bool flag = (p >= shape::green);
  bool flag = (p <= color::blue);

  enum Color { red, green, blue };
  enum Shape { circle, square, triangle };

  Color c = red;
  bool f = (c >= triangle);
}
