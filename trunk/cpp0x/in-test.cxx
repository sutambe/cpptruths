#include "in.hxx"

#include <iostream>
#include <array>

using namespace std;

struct matrix
{
  matrix () {}
  matrix (matrix const&) {cout << "copy, ";}
  matrix (matrix&&) {cout << "move, ";}

  matrix& operator+= (in<matrix> m)
  {
    cout << (m.rvalue () ? "move+=, " : "copy+=, ");
    return *this;
  }

  std::array<int, 65536> data;
};

inline matrix operator+ (in<matrix> x, in<matrix> y)
{
  return std::move (
    x.rvalue () ? x.rget () += y :
    y.rvalue () ? y.rget () += x :
    matrix (x) += y);
}

struct my_string
{
  struct null_string {};
  my_string (const char* s)
  {
    if (s != 0)
      cout << "my_string (" << s << ")" << endl;
    else
    {
      cout << "my_string (0)" << endl;
      throw null_string ();
    }
  }

  my_string (const my_string&) {cout << "my_string (copy)" << endl;}
  my_string (my_string&&) {cout << "my_string (move)" << endl;}
  ~my_string () {cout << "~my_string ()" << endl;}
};

int f (in<my_string> s)
{
  cout << "f (" << (s.rvalue () ? "rvalue" : "lvalue") << ")" << endl;
  return 0;
}

void g (int, int)
{
  cout << "g ()" << endl;
}

void m (in<my_string> s)
{
  cout << "m (" << (s.rvalue () ? "rvalue" : "lvalue") << ")" << endl;
  my_string c (s.move ());
}

int main()
{
  // Let's invoke all the possible permutations of lvalues/rvalues
  {
    matrix s1;
    matrix s2;
    matrix m1 (matrix() + s1);        cout << endl;
    matrix m2 (matrix() + matrix());  cout << endl;
    matrix m3 (s1 + s2);              cout << endl;
    matrix m4 (s1 + matrix());        cout << endl;

    // and a couple more
    matrix m5 (matrix() + matrix() + matrix()); cout << endl;
    matrix m6 (s1 + matrix() + s2 + matrix());  cout << endl;
  }

  // Test lvalue/rvalue distinction.
  //
  {
    cout << endl;
    my_string l ("lvalue");
    const my_string& lr (l);

    cout << endl;
    f (l);                     cout << endl;
    f (lr);                    cout << endl;
    f (my_string ("rvalue"));  cout << endl;
    f (std::move (l));         cout << endl;
  }

  // Test move.
  //
  {
    cout << endl;
    my_string l ("lvalue");
    cout << endl;

    m (l);                     cout << endl;
    m (my_string ("rvalue"));  cout << endl;
  }

  // Test implicit conversion.
  //
  {
    cout << endl;
    f ("implicit"); cout << endl;
    g (f ("implicit1"), 1); cout << endl;
    g (1, f ("implicit2")); cout << endl;
    g (f ("implicit1"), f ("implicit2")); cout << endl;
  }

  // Test exception in implicit conversion.
  //
  try {f ((const char*) 0);} catch (const my_string::null_string&) {}
}
