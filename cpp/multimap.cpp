#include <map>
#include <iostream>

using namespace std;

struct ltstr
{
  bool operator()(const char* s1, const char* s2) const
  {
    return strcmp(s1, s2) < 0;
  }
};

int main()
{
  multimap<const char*, int, ltstr> m;
  
  m.insert(pair<const char* const, int>("a", 1));
  m.insert(pair<const char* const, int>("c", 2));
  m.insert(pair<const char* const, int>("b", 3));
  m.insert(pair<const char* const, int>("b", 4));
  m.insert(pair<const char* const, int>("a", 5));
  m.insert(pair<const char* const, int>("b", 6));

  cout << "Number of elements with key a: " << m.count("a") << endl;
  cout << "Number of elements with key b: " << m.count("b") << endl;
  cout << "Number of elements with key c: " << m.count("c") << endl;

  cout << "Elements in m: " << endl;
  for (multimap<const char*, int, ltstr>::iterator it = m.begin();
       it != m.end();
       ++it)
   cout << "  [" << (*it).first << ", " << (*it).second << "]" << endl;

  return 0;
}

