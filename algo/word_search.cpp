//
// https://leetcode.com/problems/word-search/
// Solution accepted

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    bool check(vector<vector<char>>& board, int i, int j, int loc, string & word, vector<vector<char>> & visited)
    {
      if(board[i][j] == word[loc] && !visited[i][j])
      {
        visited[i][j] = 1;
        if(search(board, i, j, loc, word, visited))
          return true;
        visited[i][j] = 0;
      }
      return false;
    }

    bool search(vector<vector<char>>& board, int i, int j, int loc, string & word, vector<vector<char>> & visited)
    {
        std::cout << "i = " << i << ", j = " << j << " " << board[i][j] << "\n";
        if(loc == word.size()-1)
          return true;
        
        // case left
        if(j >= 1 && check(board, i, j-1, loc+1, word, visited))
          return true;
        
        // case right
        if(j < board[0].size()-1 && check(board, i, j+1, loc+1, word, visited))
          return true;

        // case up
        if(i >= 1 && check(board, i-1, j, loc+1, word, visited))
          return true;
        
        // case down
        if(i < board.size()-1 && check(board, i+1, j, loc+1, word, visited))
          return true;
        
        return false;
    }

    bool found(vector<vector<char>>& board, int i, int j, string & word)
    {
        vector<vector<char>> visited;
        visited.resize(board.size());
        for(auto & row : visited)
            row = vector<char>(board[0].size(), 0);
          
        visited[i][j] = 1;  
        int loc = 0;
        return search(board, i, j, loc, word, visited);
    }
    
    bool exist(vector<vector<char>>& board, string word) {
        int i = 0, j = 0;
        for(auto & row: board)
        {
          j = 0;
          for(char & col: row)
          {
            if(col == word[0] && found(board, i, j, word))
              return true;
            j++;
          }
          i++;
        }
        return false;
    }
};

void test1()
{
  vector<vector<char>> input;
  input.resize(3);
  for(auto & row : input)
    row.resize(4);
  char a[5] = "ABCE", b[5] = "SFCS", c[5] = "ADEE";
  std::copy(a, a+5, input[0].begin());
  std::copy(b, b+5, input[1].begin());
  std::copy(c, c+5, input[2].begin());
  
  Solution s;
  std::cout << boolalpha << s.exist(input, "ABCCED") << "\n";
  std::cout << boolalpha << s.exist(input, "SEE") << "\n";
  std::cout << boolalpha << s.exist(input, "ABCB") << "\n";
}

void test2()
{
  vector<vector<char>> input;
  input.resize(3);
  for(auto & row : input)
    row.resize(4);
  char a[5] = "ABCE", 
       b[5] = "SFES", 
       c[5] = "ADEE";
  std::copy(a, a+5, input[0].begin());
  std::copy(b, b+5, input[1].begin());
  std::copy(c, c+5, input[2].begin());
  
  Solution s;
  std::cout << boolalpha << s.exist(input, "ABCESEEEFS") << "\n";
}

int main(void)
{
  test1();
  test2();
}
