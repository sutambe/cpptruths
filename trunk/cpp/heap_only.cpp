class HeapOnly 
{
  public:
    HeapOnly() {} 
  protected:
    ~HeapOnly() {}
};
HeapOnly h1; // Destructor is protected so h1 can't be created globally. 
HeapOnly func()
{
  return *new HeapOnly;
}
struct Heap : HeapOnly {
};

struct NoHeap  {
  protected:
    static void * operator new (unsigned int);
    static void * operator new [] (unsigned int);
};

struct NoHeapTwo : NoHeap {
};

int main(void) 
{
  HeapOnly h2; // Destructor is protected so h2 can't be created on stack.
  func();
  Heap h;
  new NoHeap;
  new NoHeapTwo;
}
