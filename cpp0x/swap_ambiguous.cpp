#include <vector>

namespace N {

class Foo {
public:
  void swap(Foo &) throw() {}
};

void swap(Foo &f1, Foo &f2) throw()
{
  f1.swap(f2);
}

}

namespace std {

template<>
void swap(N::Foo &f1, N::Foo &f2) throw()
{
  f1.swap(f2);
}

}

int main(void)
{
  using std::swap;
  N::Foo f1, f2;
  swap(f1, f2);
}
