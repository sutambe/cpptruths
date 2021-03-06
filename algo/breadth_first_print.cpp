#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <algorithm>

using namespace std;

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec)
{
    for (auto& el : vec)
    {
        os << el << ' ';
    }
    return os;
}

struct Node 
{
    int value;
    Node *left, *right;

    Node(int i)
     : value(i), left(0), right(0)
    {}
};

void breadth_first_print(Node &root)
{
    std::vector<Node *> storage1;
    std::vector<Node *> storage2;
    
    std::vector<Node *> *parent = &storage1;
    std::vector<Node *> *children = &storage2;
    
    parent->push_back(&root);
    std::cout << root.value << std::endl;

    while(parent->size())
    {
        std::for_each(parent->begin(), 
                      parent->end(),
                      [children](Node *n)
                        {
                            if(n->left)  children->push_back(n->left);
                            if(n->right) children->push_back(n->right);
                        });
        std::for_each(children->begin(),
                      children->end(),
                      [](Node *n) {
                        std::cout << n->value << " ";  
                      });
        std::cout << std::endl;
        std::swap(parent, children);
        children->clear();
    }
}

void level_order_reverse(Node & root)
{
  stack<queue<Node *>> stack;
  queue<Node *> s1, s2;
  queue<Node *> *qp = &s1, *qc = &s2;
  qp->push(&root);
  stack.push(*qp);
  
  while(!qp->empty())
  {
    while(!qp->empty())
    {
      Node * node = qp->front(); qp->pop();
      if(node->left) qc->push(node->left);      
      if(node->right) qc->push(node->right);
    }
    stack.push(*qc);
    std::swap(qp, qc);
  }
  
  while(!stack.empty())
  {
    queue<Node *> & q = stack.top(); 
    while(!q.empty())
    {
      std::cout << q.front()->value << " ";
      q.pop();
    }
    stack.pop();
    std::cout << "\n";
  }
}

void breath_first_average(Node & root)
{
  
}

int main()
{
    const int SIZE = 11;
    Node nodes[SIZE] = { 0, 7, 5, 9, 3, 6, 8, 11, 2, 4, 21 };

    nodes[1].left  = &nodes[2];
    nodes[1].right = &nodes[3];
    nodes[2].left  = &nodes[4];
    nodes[2].right = &nodes[5];
    nodes[3].left  = &nodes[6];
    nodes[3].right = &nodes[7];
    nodes[4].left  = &nodes[8];
    nodes[4].right = &nodes[9];
    nodes[7].right = &nodes[10];

    breadth_first_print(nodes[1]);
    level_order_reverse(nodes[1]);

}
