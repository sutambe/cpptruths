#include <stack>
#include <string>
#include <iostream>

#ifndef MAIN
#define MAIN main
#endif

using namespace std;

bool is_operator(char c)
{
  switch(c)
  {
    case '+':
    case '-':
    case '*':
    case '/':
    case '^':
      return true;
    default:
      return false;
  }
}

bool gte(char a, char b)
{
  if (a==b)
    return true;
  else if(a=='*' && b=='+')
    return true;
  else
    return false; 
}

void process(stack<char> & op, stack<int> & digits)
{
  char c = op.top(); op.pop();
  int a = digits.top(); digits.pop();
  int b = digits.top(); digits.pop();

  switch(c)
  {
    case '+':
      digits.push(a+b);
      break;
    case '-':
      digits.push(b-a);
      break;
    case '*':
      digits.push(a*b);
      break;
    case '/':
      digits.push(b/a);
      break;
    case '^':
      int value = 1;
      for(int i = 0; i < a; i++)
        value *= b;
      digits.push(value);
      break;
  }
}

int eval(const string & expr)
{
  stack<char> op;
  stack<int> digits;
  
  for(char c : expr)
  {
    if(c >= '0' && c <= '9')
      digits.push(c-'0');
    else if(is_operator(c) && op.empty())
      op.push(c);
    else if(is_operator(c) && !op.empty())
    {
      while(!op.empty() && gte(op.top(), c))
        process(op, digits);
      op.push(c);
    }
  }
  while(!op.empty())
    process(op, digits);
 
  return digits.top();
}

int MAIN(int argc, char *argv[])
{
  cout << eval(argv[1]) << "\n";
  return 0;
}

