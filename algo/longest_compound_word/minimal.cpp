#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <ctime>

using namespace std;

struct TriNode
{
  char data;
  bool end;
  auto_ptr<TriNode> children[26];

  TriNode(char c)
    : data(c), end(false)
  {}

};

class Tree {

  TriNode root;

public:
  Tree() : root(0) {}

  void insert(const string & str)
  {
    TriNode *node = &root;
    for(int i=0;i < str.size(); i++)
    {
      char c = str[i];
      if (!node->children[c - 'a'].get())
        node->children[c - 'a'] = auto_ptr<TriNode>(new TriNode(c));
    
      node = node->children[c - 'a'].get();
    }
    node->end = true;
  }

  bool is_compound(const string & str, size_t index = 0) const
  {
    const TriNode * node = &root;
    for (size_t i = index; i < str.size() && node; i++)
    {
      if (node->children[str[i] - 'a'].get())
      {
        node = node->children[str[i] - 'a'].get();
        if (node->end)
        {
          if (i == str.size() - 1)
          {
            return index!=0;
          }
          else if (is_compound(str, i + 1))
            return true;
        }
      }
      else
        return false;
    }
    return false;
  }
};

int main(int argc, char * argv[])
{
  try {
    time_t start, end;
    time(&start);

    ifstream input(argv[1]);
    string line;
    Tree t;
    vector<string> all;
    all.reserve(173530);

    while (input >> line)
    {
      t.insert(line);
      all.push_back(line);
    }

    int count = 0, first = 0, second = 0;
    for(int i=0;i < all.size(); i++)
    {
      if (t.is_compound(all[i]))
      {
        count++;
        if(all[first].size() < all[i].size())
        {
          second = first;
          first = i;
        }
      }
    }
    time(&end);
    cout << "Longest = " << all[first] << "\n";
    cout << "2nd longest = " << all[second] << "\n";
    cout << "Total = " << count << "\n";
    cout << "elapsed time = " << end-start << "ms\n";
  }
  catch(std::exception & ex)
  {
    cout << "Opps! an exception: " << ex.what() << "\n";
  }
}
