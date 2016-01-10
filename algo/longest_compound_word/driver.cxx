#include <numeric>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <stack>
#include <tuple>
#include <chrono>

using namespace std;

struct TriNode
{
  char data;
  bool end;
  int height;
  shared_ptr<TriNode> children[26];

  TriNode(char c)
    : data(c), end(false), height(0)
  {}

};

class Tree {

  TriNode root;

  static void depth_first(const TriNode * node, char *word, int i)
  {
    if (node)
    {
      word[i] = node->data;
      if (node->end)
      {
        word[i + 1] = 0;
        cout << word << " " << node->height << "\n";
      }
      for (shared_ptr<TriNode> alpha : node->children)
      {
        depth_first(alpha.get(), word, i + 1);
      }
    }
  }

  static void update_heights(stack<TriNode *> & stack)
  {
    while (!stack.empty())
    {
      TriNode *top = stack.top(); stack.pop();
      int max = -1;
      for (shared_ptr<TriNode> & node : top->children)
      {
        if (node && max < node->height)
          max = node->height;
      }
      top->height = max + 1;
    }
  }


  static void recurse_highest(const TriNode * node, char *word, int i)
  {
    int max_index = 0;
    int max_height = -1;
    if (node && node->height >= 1)
    {
      for (int a = 0; a < 26; a++)
      {
        if (node->children[a] && (node->children[a]->height > max_height))
        {
          max_height = node->children[a]->height;
          max_index = a;
        }
      }
      word[i] = node->children[max_index]->data;
      word[i + 1] = 0;
      recurse_highest(node->children[max_index].get(), word, i + 1);
    }
  }

public:
  Tree() : root(0) {}

  void insert(const string & str)
  {
    stack<TriNode *> stack;
    TriNode *node = &root;
    stack.push(node);
    for (char c : str)
    {
      if (!node->children[c - 'a'])
        node->children[c - 'a'] = make_shared<TriNode>(c);
    
      node = node->children[c - 'a'].get();
      stack.push(node);
    }
    node->end = true;
    update_heights(stack);
  }

  bool find(const string & str) const
  {
    const TriNode *node = &root;
    for (char c : str)
    {
      if (node->children[c - 'a'] && node->children[c - 'a']->data == c)
        node = node->children[c - 'a'].get();
      else
        return false;
    }
    return node->end;
  }

  void print() const
  {
    for (shared_ptr<TriNode> node : root.children)
    {
      char word[128] = { 0 };
      int i = 0;
      depth_first(node.get(), word, i);
    }
  }

  string get_longest() const
  {
    char word[128] = { 0 };
    int i = 0;
    recurse_highest(&root, word, i);
    return word;
  }

  bool is_compound(const string & str, size_t index = 0) const
  {
    const TriNode * node = &root;
    for (size_t i = index; i < str.size() && node; i++)
    {
      if (node->children[str[i] - 'a'])
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
  std::chrono::time_point<std::chrono::system_clock> start, end;
  start = std::chrono::system_clock::now();

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

  int count = 0;
  string first, second;
  for(int i=0;i < all.size(); i++)
  {
    if (t.is_compound(all[i]))
    {
      count++;
      if(first.size() < all[i].size())
      {
        second = first;
        first = all[i];
      }
    }
  }
  
  end = std::chrono::system_clock::now();
  cout << "Longest = " << first << "\n";
  cout << "2nd longest = " << second << "\n";
  cout << "Total " << count << "\n";
  std::chrono::duration<double> elapsed_seconds = end - start;
  cout << "elapsed time = " << elapsed_seconds.count() << "s\n";
  cout << "Longest (via height) = " << t.get_longest() << "\n";
}