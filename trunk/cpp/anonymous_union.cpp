namespace {

static union  //no tag name
{
  int x;
  char c[4];
}; //no named instances or pointers
int first_byte=c[0];

}

class C
{
public:
 static struct { 
 short len; 
 char array[10];
 };
 C(int x) : len(x) {}
};

// anonynous structs work on VC2010 and gcc
// does not work on clang++ and Comeau

int main(void)
{
  x=5; //members of an anonymous union are accessed directly
  C c(10), d(20);
  c.len = d.len;
  return 0;
}
