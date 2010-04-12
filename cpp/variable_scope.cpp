#include <iostream>

using namespace std;

int main(void)
{
    if (int t=1) 
    {
       cout << "t > 0 = " << t << endl;
    }
    else if (t == 2)
    {
       cout << "t==2 " << endl;
       t--;
    }
    else
    {
       cout << "t==0 " << endl;
       t++;
    }
    std::cout << t << std::endl; // t is not available here.
    return 0;
}
