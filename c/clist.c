#include <stdio.h>
#include <assert.h>
#include "generic_list.h"

typedef struct Node_tag
{
  Glist_element _element;
  int value;  
} Node;

void NodeCopy(Node *dest, Node *src)
{
  dest->value = src->value;
}

void NodeDestroy(Node * n)
{
  // No-op.
}

int main (void)
{
  GLIST(Node) mylist;
  Glist_initialise (&mylist);
  Node n1;
  assert(&n1._element.next == &n1);
  n1.value = 10;
  Glist_add(&mylist, &n1, sizeof(Node), NodeCopy);
  n1.value = 20;
  Glist_add(&mylist, &n1, sizeof(Node), NodeCopy);
  Glist_iterator iter = Glist_itr_begin(&mylist);
  for (;iter;iter = Glist_itr_next(iter))
  {
    Node *n = (Node *) iter;
    printf("%d\n",n->value);
  }

  Glist_deinitialise(&mylist, NodeDestroy);
  return 0;
}
