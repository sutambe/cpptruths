#include <stdio.h>

typedef struct Node_tag
{
  struct Node_tag * next, * prev;
  int data;
} Node;

typedef struct List_tag
{
  Node head;
} List;

List create_list()
{
  List l = { { &l.head, &l.head, 100 } };
  return l;
}

int main(void)
{
  //List l = { { &l.head, &l.head, 100 } };
  List l = create_list();
  Node * temp = &l.head;
  while(temp)
  {
    printf("%d\n", temp->data);
    temp=temp->next;
  }

  return 0;
}
