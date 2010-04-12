#include <iostream>
#include <limits>

int main (void)
{
  float NaN = std::numeric_limits<float>::quiet_NaN();
  if (NaN != NaN)
    std::cout << "NaN = " << NaN << std::endl;

  return 0;
}
