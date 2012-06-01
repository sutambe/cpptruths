#include <string>
#include <cstdio>

struct Thing {
 virtual void operator+(std::string) 
 {
  printf("Thing member operator +\n");
 }
};
 
void operator+(Thing&, const char*) 
{
  printf("free operator +\n");
}

struct Blob: public Thing 
{ 
 virtual void operator+(std::string) 
 {
  printf("Blob member operator +\n");
 }
};

int main(void)
{
  Thing t;
  t+"T";
  Blob b;
  Thing & t2 = b;
  std::string s("S");
  t2+s;
}
