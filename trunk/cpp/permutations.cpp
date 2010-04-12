#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include <set>

int main()
{
    std::string input="123";
    std::vector<std::string> perms;
    perms.push_back(input);
    std::string::iterator itBegin = input.begin();
    std::string::iterator itEnd = input.end();
    while(std::next_permutation(itBegin, itEnd))
    {
        perms.push_back(std::string(itBegin, itEnd));
    }
    std::copy(perms.begin(), perms.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
}
