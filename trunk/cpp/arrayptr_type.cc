#include <iostream>

using namespace std;

struct abc
{
    abc(){ cout << "abc" << endl; }
};

static abc a1;

int main(int argc, char **argv, char *env[])
{
    int (*ptr_array)[20];
    int myarray[20];
    int *ptr_int;
    void *v=0;
    ++v;

    cout << v << endl;

    //ptr_array=new int[20];
    ptr_array=&myarray;
    cout << (unsigned)ptr_array << endl;
    ptr_array++;
    cout << (unsigned)ptr_array << endl;

    ptr_int=myarray;

    return 0;
}

