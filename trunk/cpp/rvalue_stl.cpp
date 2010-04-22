#include <iostream>
#include <vector>

struct X {
  X() {
    std::cout << "X::X()\n";
  }
  X(const X &) {
    std::cout << "X::X(const X &)\n";
  }
};

std::vector<X> vmaker(int i) {
  return std::vector<X>(i);
}

int main(void)
{
  std::vector<X> vx;
  vx = vmaker(10);
}
