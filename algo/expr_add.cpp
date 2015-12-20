#define MAIN removed_main
#include "eval_infix.cpp"

#include <vector>

class Solution {
public:
    void back_track(int target, const string & num, int index, const string & expr, vector<string> & answer)
    {
      if(index < num.size()-1)
      {
        back_track(target, num, index+1, expr + "+" + num[index+1], answer);
        back_track(target, num, index+1, expr + "*" + num[index+1], answer);
      }
      else
        if(target == eval(expr))
          answer.push_back(expr);
    }
    vector<string> addOperators(string num, int target) 
    {
        vector<string> answer;
        string expr { num[0] };
        back_track(target, num, 0, expr, answer);
        return answer;
    }
};

int main(int argc, char * argv[])
{
  Solution s;
  vector<string> answer = s.addOperators(argv[1], stoi(argv[2]));
  for(auto & s : answer)
  {
    cout << s << " ";
  }
  cout << "\n";
  return 0;
}

