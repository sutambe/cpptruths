#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

class Solution {
public:
    int minPathSum(vector<vector<int>>& grid) {
        vector<vector<int>> cost;
        int m = grid.size();
        int n = grid[0].size();
        cost.resize(m+1);
        for(int i=0;i <= m ; i++)
            cost[i].resize(n+1);
        for(int i = 0; i <= m; i++)
            cost[i][n] = 0x7FFFFFFF;
        for(int j = 0; j <= n; j++)
            cost[m][j] = 0x7FFFFFFF;

        cout << "here1\n";
        for(int i=m-1; i >= 0; i--)  
        {
            for(int j=n-1; j >= 0; j--)  
            {
                if((i == m-1) && (j == n-1))
                    cost[i][j] = grid[i][j];
                else
                    cost[i][j] = grid[i][j] + min(cost[i+1][j], cost[i][j+1]);
            }
        }
        
        for(auto & row : cost)
        {
          for(auto & cell : row)
            cout << cell << " ";
          cout << endl;
        }
        
        cout << "here2\n";
        vector<pair<int, int>> path;
        int i = 0, j = 0, sum = cost[i][j];
        path.push_back(make_pair(i, j));
        while(!((i == m-1) && (j == n-1)))
        {
            if(cost[i+1][j] > cost[i][j+1])
            {
                sum += cost[i][j+1];
                path.push_back(make_pair(i, j+1));
                j++;
            }
            else
            {
                sum += cost[i+1][j];
                path.push_back(make_pair(i+1,j));
                i++;
            }
        }
        return sum;
    }
    
};

int main(void)
{
  vector<vector<int>> grid(1);
  grid[0].resize(1);
  
  Solution s;
  cout << s.minPathSum(grid) << endl;
}