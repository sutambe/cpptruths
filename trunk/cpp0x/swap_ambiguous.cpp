#include <iostream>
#include <vector>
#include <algorithm>

namespace N {

class Foo {
public:
  void swap(Foo &) throw() {}
};

void swap(Foo &f1, Foo &f2) throw()
{
  std::cout << "namespace swap\n";
  f1.swap(f2);
}
}

namespace std {

template<>
void swap<N::Foo>(N::Foo &f1, N::Foo &f2) noexcept
{
  f1.swap(f2);
}

}

int main(void)
{
  using std::swap;
  N::Foo f1, f2;
  swap(f1, f2);
  N::Foo fa1[5], fa2[5];
  swap(fa1, fa2);
}
