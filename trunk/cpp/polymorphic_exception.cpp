#include <iostream>

template <class T>
void raise()
{
  throw T();
}
struct BaseException {};
struct DerivedException : BaseException {};

void foo (void)
{
  throw;
}

int main(void)
try
{
  try {
    throw DerivedException();
  }
  catch (BaseException & e)
  {
    std::cout << "Calling foo\n";
    foo();
  }
}
catch (DerivedException & e)
{
  std::cout << "DerivedException\n";
}
catch (BaseException & e)
{
  std::cout << "BaseException\n";
}
