#include <string>

typedef struct {
    std::string keyset[2];
    int linkType;
} keywords;

typedef struct {
    keywords keys;
    std::string response[3];
} Record;

int main (void)
{
  int N = { 1 };
  int P = 2;
  Record r1 = { 
                 { { "Archana" }, 10 }, // Incomplete initializer.
                 { "Sumant", "Uday", "Tambe"  } 
              };

  Record r2[2] = { 
                  { 
                    { {"Archana", "Potnis" }, 10 }, 
                    { "Sumant", "Uday", "Tambe"  } 
                  },
                  { 
                    { {"Archana", "Potnis" }, 10 }, 
                    { "Sumant", "Uday", "Tambe"  } 
                  }
                 };

  return 0;
}

