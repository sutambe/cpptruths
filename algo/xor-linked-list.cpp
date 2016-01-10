#include <iostream>
#include <cstdio>

#define XOR(a,b) ((unsigned)a ^ (unsigned)b)

struct Node
{
  int data;
  unsigned ptrdiff; 
  // ptrdiff is XOR of prev and next pointers
  // You can also traverse in reverse direction.
};  

struct List
{
  Node * head;
  Node * tail;
};

List new_list(unsigned int num)
{
  List l = { 0, 0 };
  if(num <= 0)
    return l;
  
  if(num >= 1)
  {
    l.head = new Node();
    l.head->data = 99;
    l.head->ptrdiff = 0;
    l.tail = l.head;
  }
  
  unsigned prev = 0;
  Node *current = l.head;
  for(int i=1; i < num; ++i)
  {
    Node * node = new Node();
    node->data = 99;
    current->ptrdiff = XOR(prev, node);
    node->ptrdiff = 0;
    prev = (unsigned) current;
    current = node;
  }
  l.tail = current;
  return l;
}

void print(List l)
{
  if(l.head)
  {
    if(l.head == l.tail)
      printf("%d ", l.head->data);
    else
    {
      unsigned prev = 0;
      Node * current = l.head;
      Node * temp = 0;
      do
      {
        printf("%d ", current->data);
        temp = (Node *)prev;
        prev = (unsigned) current;
        current = (Node *) XOR(temp, current->ptrdiff);
      }
      while(temp != current);
    }
  }
}

int main(void)
{
  List l = new_list(10);
  print(l);
}