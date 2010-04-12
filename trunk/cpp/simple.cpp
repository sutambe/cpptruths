#include <iostream>
#include <conio.h>

using namespace std;
int i=10;

int main(void)
{
    string myname;
    cout << "Hello World" << endl;
    cin >> myname;
    
    cout << (void *)cout.rdbuf();
    
    cout << "My name is: " << myname << endl;
    getch();
    
    return 0;
}

