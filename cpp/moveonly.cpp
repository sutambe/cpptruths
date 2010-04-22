#include <memory>
#include <vector>

struct MoveOnly
{
  MoveOnly() {}
  MoveOnly(MoveOnly && other) {}
  MoveOnly & operator = (MoveOnly && m) 
  {
    return *this;
  }

  //private:

  MoveOnly(const MoveOnly & other) = delete;
  MoveOnly & operator = (const MoveOnly &) = delete;
};

void acceptRValue(MoveOnly && mo) {}

int main()
{
  acceptRValue(MoveOnly()); 
  MoveOnly m;
  m = std::move(m);

  acceptRValue(std::move(m));
  // MoveOnly && m2 = m; // Error as expected.
  
  typedef std::unique_ptr<int> int_ptr;

  int_ptr pi(new int(1));

  std::vector<int_ptr> vec;

  vec.push_back(std::move(pi));  
  vec.push_back(int_ptr(new int(2)));
}
