#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
using namespace std;


// As this class manages its own memory and must also work with STL
// it must implement the copy-and-swap idiom for strong exception-safety.
//
class Node_Class
{
  public:
    int id;
    float *x;
    size_t size; // need to know size as size of the dynamically allocated memory varies.
    Node_Class(); 
    Node_Class(const Node_Class &);
    Node_Class & operator = (const Node_Class &);
    Node_Class(int,float,float);
   ~Node_Class() throw();
    int get_id() const;
    void set(int,float,float);
    void swap(Node_Class &) throw();
    friend ostream & operator << (ostream &, const Node_Class &);
};

Node_Class::Node_Class()  // default constructor is of almost negligible overhead and does no allocation.
  : id (-1),
    x (0),
    size (0)
{ }

Node_Class::Node_Class(int ID, float X, float Y)  // allocates!
  : id (ID),
    x (new float[2]),
    size (2)
{
  x[0] = X; 
  x[1] = Y;
}


// destructor should never throw and no need to check x==0 as the default constructor 
// is making it zero. Other idioms in this program will never make it something nonsensical.
Node_Class::~Node_Class() throw() 
{ 
  delete [] x;
  /* if (x!=0) { delete [] x; x = 0; }*/
}

// copy constructro should always make a deep copy. Particularly the
// dynamically allocated memory.
Node_Class::Node_Class(const Node_Class & n)  
  : id (n.id),
    x (new float[n.size]),
    size (n.size)
{ 
  std::copy(n.x, n.x + n.size, x);
}

// The copy-and-swap idiom.
// Assignment operator should also do a deep copy and should use the copy constructor 
// to do this job in exact those three steps. No worries even if memory allocation fails.
// The left-hand-side and right-hand-side objects will stay as they were.
Node_Class & Node_Class::operator = (const Node_Class & n)
{
  Node_Class temp(n); // copy constructor.
  temp.swap(*this);   // Swap the contents with temp.
  return *this;       
}

// swap never throws. Simply swaps the contents.
void Node_Class::swap(Node_Class & n) throw()
{
  std::swap(id, n.id);
  std::swap(x, n.x);
  std::swap(size, n.size);
}

bool comp (Node_Class n1, Node_Class n2) { return n1.id < n2.id; }

int Node_Class::get_id() const
{
  return id;
}

void Node_Class::set(int ID, float X, float Y)
{
  Node_Class temp(ID, X, Y); // creates a new object
  temp.swap(*this); // then swaps it with the old. memory of old will be deleted, if any.
/*  
  id = ID;
  if (x==0) x = new float[2];
  x[0] = X;
  x[1] = Y;
*/
}

ostream & operator << (ostream & o, const Node_Class & n)
{
  std::copy(n.x, n.x + n.size, ostream_iterator<float>(std::cout, ", "));
  return o;
}
int main()
{
  /*
  vector<Node_Class> Node_Vector;
  Node_Class node;
  for (int inode=0;inode<1;inode++)
  {
    node.set(inode,float(inode),float(inode));
    Node_Vector.push_back(node);
  }
*/
  // The following code is equivalent (and as fast) to your previous code with respect to
  // the number of dynamic allocations. But it is a lot safer in terms of 
  // allocating and deleting memory.
  size_t size = 10;
  vector<Node_Class> node_vector(size); // Only invokes the default constructor so should be fast. No dynamic memory.
  size_t inode = 0;
  for(vector<Node_Class>::iterator iter(node_vector.begin());
      iter != node_vector.end();
      ++iter, ++inode)
  {
    Node_Class node(inode, inode, inode); // Create a temporary. Allocates memory dynamically.
    //iter->swap(node); // Just swap with each default constructed object. 
    *iter = node;
  }
  std::copy(node_vector.begin(), node_vector.end(),std::ostream_iterator<Node_Class>(std::cout,"\n"));
}
