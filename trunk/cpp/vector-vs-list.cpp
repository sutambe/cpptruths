#include <iostream>
#include <vector>
#include <list>
#include <iterator>
#include <stdlib.h>

template <class FWIter>
void print(FWIter begin, FWIter end)
{
  std::copy(begin, end,
      std::ostream_iterator<typename FWIter::value_type>(std::cout, " "));
  std::cout << std::endl;
}

void list_insert_sort(size_t N)
{
  typedef std::list<int> T;
  T list;
  list.push_back(rand() % 50);
  size_t count = 1;

  //print(list.begin(), list.end());

  for(size_t i = 1;i < N;++i)
  {
    int value = rand() % 50;

    T::iterator iter = list.begin();
    for(size_t pos = 0; (*iter < value) && (pos < count); ++iter, ++pos);
    T::iterator location = iter;

    list.insert(iter, value);

    ++count;
    //print(list.begin(), list.end());
  }
}

void vector_insert_sort(size_t N)
{
  typedef std::vector<int> T;
  T vector(N);
  *(vector.begin()) = rand() % 50;
  size_t count = 1;

  //print(vector.begin(), vector.end());

  for(size_t i = 1;i < N;++i)
  {
    int value = rand() % 50;

    T::iterator iter = vector.begin();
    for(size_t pos = 0; (*iter < value) && (pos < count); ++iter, ++pos);
    T::iterator location = iter;

    if(iter == vector.end())
      *iter = value;
    else
    {
      iter = vector.end();
      do
      {
        T::iterator next = iter;
        --iter;
        *next = *iter;
      }
      while(location != iter);
      
      *location = value;
    }

    ++count;
    //print(vector.begin(), vector.end());
  }
}

int main(int argc, char * argv[])
{
  time_t t;
  time(&t);
  srand(t);

  if(argc <= 2) {
    std::cout << "Usage: vector-vs-list <size> <list|vector>\n";
    return 1;
  }

  if(std::string(argv[2]) == "list")
    list_insert_sort(atoi(argv[1]));

  if(std::string(argv[2]) == "vector")
    vector_insert_sort(atoi(argv[1]));

  return 0;
}
