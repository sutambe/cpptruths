#include <iostream>
#include <string>

#define named(blockname) goto blockname; \
                         blockname##_skip: if (0) \
                         blockname:

#define break(blockname) goto blockname##_skip; 

using namespace std;

struct test
{       
  std::string str;
  test (std::string s) : str(s) { cout << "test::test()::" << str << "\n"; } 
  ~test () { cout << "~test::" << str << "\n"; } 
};

int main(void)
{
  
  named (outer) for (int i = 0; i < 10; i++)
  {
    test t1("t1");
    int j =0;
    for (test t2("t2"); j < 5; j++)
    {           
      test t3("t3");
      if (j == 2) break(outer);
    }
    std::cout << "after inner\n";
  }

  return 0;
}

