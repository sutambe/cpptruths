#include <iostream>
#include <string>

using namespace std;

void foo(string s)
{        
    cout<<(void*)s.data()<<endl;        
    string ss=s;        
    cout<<(void*)ss.data()<<endl;        
    char p=ss[0];        
    cout<<(void*)ss.data()<<endl;
}

int main()
{        
    //make sure the string is longer than the size of potential   
    //implementation of small-string.   
    std::string s = "012345678901234567890123456789012345678901234567890123456789"
                    "012345678901234567890123456789012345678901234567890123456789"
                    "012345678901234567890123456789012345678901234567890123456789"
                    "012345678901234567890123456789012345678901234567890123456789"
                    "012345678901234567890123456789012345678901234567890123456789";
    cout<<(void*)s.data()<<endl;        
    foo(s);        
    cout<<(void*)s.data()<<endl;
}

/*
MS Visual C++ 2010 --> Does NOT have copy-on-write strings.

0044FB8C
0044FB6C
0044FB40
0044FB40
0044FB8C

*/

/* 
Cygwin g++ (GCC) 4.3.4 20090804 (release) 1     --> Has copy-on-write strings.
Linux  g++ (Ubuntu/Linaro 4.5.2-8ubuntu4) 4.5.2 --> Has copy-on-write strings.

0xbd0314
0xbd0314
0xbd0314
0xbe0364
0xbd0314

*/

