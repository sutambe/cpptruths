#include <list>
#include <vector>
#include <iostream>

/*
template <class T>
struct Node
{
  Node * next, prev;
  T value;
};

template <class T>
class LinkedList
{
    Node *head, *tail, *end;
    size_t size;
  public:
    LinkedList()
      : head(0), tail(0), end(0), size(0)
    {  
    }

    LinkedList(size_t s)
      : head(0), tail(0), end(0), size(0)
    {
      if (s > 0)
      {
        alloc_first_node();
        Node *temp = head;
        for (int i = 1;i < s;i++)
        {
          temp->next = static_cast<Node *>(operator new (sizeof (Node)));
          temp->next->prev = temp; 
          temp->next->next = 0;
          temp = temp->next;
          end = temp;
        }
      }
    }
    void puch_back(T const & val)
    {
      if (tail == 0)
      {
        create_first();
      }
      new (&tail->prev->value) T(val);
    }
    void alloc_first_node()
    {
        head = static_cast<Node *>(operator new (sizeof (Node)));
        head->prev = 0;
        head->next = 0;
        tail = head;
        end = head;
    }
};
*/
class Foo
{
  public:
    //Foo () {}
    Foo (int i) {}
    ~Foo ()  
    { std::cout << "Foo destructor called." << std::endl; }
};

int main (void)
{
  std::vector<Foo> v;
  v.reserve(100);
  std::cout << v.size() << std::endl;
  std::list<Foo> l1;      // does not require default-constructible.
  //l1.reserve(100);
  //std::list<Foo> l2(100); // requires default-constructible. Not sure why.
  //std::cout << l1.size() << std::endl;
  //std::cout << l2.size() << std::endl;

  return 0;
}
