#include <iostream>
#include <iterator>

using namespace std;

int A[] = {4, 3, 2, 1, 0};
const int N = sizeof(A) / sizeof(int);

template <class BidirectionalIterator> 
void snail_sort(BidirectionalIterator first, BidirectionalIterator last)
{
  do
  {
    copy (A, A+N, ostream_iterator <int> (cout, " "));
    cout << endl;
  }
  while (next_permutation(first, last));
}

int main()
{
  next_permutation (A, A+N);
  snail_sort(A, A+N);
  //copy(A, A+N, ostream_iterator<int>(cout, "\n"));
}

