#include <iostream>
#include <vector>
#include <list>
#include <iterator>
#include <string>
#include <stdlib.h>
#include <ctime>

int gen_random()
{
  return rand() % 10000;
}

template <class FWIter>
void print(FWIter begin, FWIter end)
{
  //std::copy(begin, end, std::ostream_iterator<typename FWIter::value_type>(std::cout, " "));
  //std::cout << std::endl;
}

void list_insert_sort(size_t N)
{
  typedef std::list<int> T;
  T list;
  list.push_back(gen_random());
  size_t count = 1;

  print(list.begin(), list.end());

  for(size_t i = 1;i < N;++i)
  {
    int value = gen_random();

    T::iterator iter = list.begin();
    for(size_t pos = 0; (*iter < value) && (pos < count); ++iter, ++pos);
    T::iterator location = iter;

    list.insert(iter, value);

    ++count;
    print(list.begin(), list.end());
  }
}

void vector_insert_sort(size_t N)
{
  typedef std::vector<int> T;
  T vector;
  vector.reserve(N);
  vector.push_back(gen_random());
  size_t count = 1;

  print(vector.begin(), vector.end());

  for(size_t i = 1;i < N;++i)
  {
    int value = gen_random();

    T::iterator iter = vector.begin();
    for(size_t pos = 0; (*iter < value) && (pos < count); ++iter, ++pos);
    T::iterator location = iter;

    if(iter == vector.end())
      vector.push_back(value);
    else
    {
      vector.resize(vector.size()+1);
      int v1 = *iter;
      do
      {
        int v2 = *(iter+1);
        *(iter+1) = v1;
        ++iter;
        v1 = v2;
      }
      while((iter+1) != vector.end());
      
      *location = value;
    }

    ++count;
    print(vector.begin(), vector.end());
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
