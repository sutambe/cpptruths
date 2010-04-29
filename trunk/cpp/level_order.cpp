#include <iostream>
#include <queue>

struct Node {
  int val;
  Node * left;
  Node * right;

  Node(int i, Node *l = 0, Node *r = 0) : val(i), left(l), right(r) {}
  ~Node() { delete left; delete right; }
};

void level_order(Node * r)
{
  std::queue<Node *> q;
  q.push(r);
  int current = 1;
  int next = 0;
  while(!q.empty()) {
    Node * t = q.front();
    q.pop();
    std::cout << t->val;
    --current;
    if(t->left) { 
      q.push(t->left); 
      next++; 
    }
    if(t->right) { 
      q.push(t->right); 
      next++; 
    }
    if(!current) {
      std::cout << std::endl;
      std::swap(current, next);
    }
  }
}

int main (void)
{
  Node root(1, new Node(2, new Node(3)), new Node(4, new Node(5), new Node(6)));
  level_order(&root);
}
