// Given an unsorted array nums, reorder it in-place such that nums[0] <= nums[1] >= nums[2] <= nums[3]....
// For example, given nums = [3, 5, 2, 1, 6, 4], one possible answer is [1, 6, 2, 5, 3, 4].

#include <fstream>
#include <vector>

using namespace std;

struct Pair {
  int min_i;
  int max_i;
  Pair(int min, int max) : min_i(min), max_i(max) {}
};
class Solution {
public:
    Pair minmax(int *A, int size)
    {
        if(size == 1) return Pair(0,0);
        bool even = (size %2) == 0;
        int begin = 0, end = even ? size : size-1;
        int max = -9999999; int max_i = -1;
        int min = 0x7FFFFFFF; int min_i = -1;
        int i = 0;
        for(i;i < end;i+=2)
        {
            if(A[i] >= A[i+1]) {
                if(A[i] > max) { max = A[i]; max_i = i; }
                if(A[i+1] < min) { min = A[i+1]; min_i = i+1; }
            }
            else {
                if(A[i] < min) { min = A[i]; min_i = i; }
                if(A[i+1] > max) { max = A[i+1]; max_i = i+1; }
            }
        }
        
        if(!even) {
            // Fixed i+i to i because by the time loop ends, i==end
            if(A[i] > max) max_i = i;
            if(A[i] < min) min_i = i;
        }
        return Pair(min_i, max_i);
    }
    void wiggleSort(vector<int>& nums) {
        if(nums.size() >= 1) {
            bool even = (nums.size() %2) == 0;
            int begin = 0, end = even? nums.size() : nums.size()-1;
            for(int i = begin; i < end; i+=2) {
                int * A = nums.data() + i;                
                Pair mm = minmax(A, nums.size()-i);
                printf("%d %d\n", A[mm.min_i], A[mm.max_i]);
                swap(A[0], A[mm.min_i]);
                swap(A[1], A[mm.max_i]);
            }
        }
    }
};

int main(int argc, char * argv)
{
  ifstream input("wiggle_sort.txt");
  vector<int> data;
  int i;
  while (input >> i)
  {
    data.push_back(i);
  }
  Solution s;
  s.wiggleSort(data);
  for(int v : data)
    printf("%d ", v);
}