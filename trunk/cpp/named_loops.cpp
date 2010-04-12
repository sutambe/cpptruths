#include <iostream>

#define named(blockname) goto blockname; \
                         blockname##_skip: if (0) \
                         blockname:

#define break(blockname) goto blockname##_skip; 

using namespace std;

struct test
{       
  ~test () { cout << "~test\n";} 
};

int main(void)
{
  
  named (outer) for (int i = 0; i < 10; i++)
  {
    test t;
    for (int j = 0; j < 5; j++)
    {           
      test t;
      if (j == 2) break(outer);
    }
  }
  return 0;
}

