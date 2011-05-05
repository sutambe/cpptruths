#include <cstdlib>
/* type of throw something; is void. 
 * Then how does it work in a ternary operator? */
void voider(void) {}
int f2(bool v) { return v ? voider() : 0; } // Not allowed
int f1(bool v) { return v ? throw 0 : 0; }  // Allowed. Why?
int main(int, char *argv[])
{
  decltype(throw 0) *i;
  return f2(atoi(argv[1]));
}
