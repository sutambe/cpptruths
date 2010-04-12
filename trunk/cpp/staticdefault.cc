#include <iostream>
using namespace std;

int lastIndent; 
ostream& indent(int chars = lastIndent) 
{ 
      lastIndent = chars; 
      while (chars-->0)
      { 
           cout.put(' '); 
      } 
      return cout; 
}
int main(void)
{
	indent(10);
	indent();
}

