#include <iterator>
#include <fstream>
#include <algorithm>

int main(void)
{
  std::ifstream in("binary_copy", std::ios_base::binary);
  std::ofstream out("binary_copy_2", std::ios_base::binary);
  std::copy(std::istreambuf_iterator<char>(in.rdbuf()),
            std::istreambuf_iterator<char>(),
            std::ostreambuf_iterator<char>(out));

  return 0;
}

