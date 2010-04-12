#include <iostream>

using namespace std;

struct base
{
    base(){ cout << "Inside base constructor" << endl; }
    void bfunc(void) { cout << "Inside bfunc" << endl; }
    virtual ~base() { cout << "Inside ~base" << endl; }
    virtual void myfunc(void)=0; 
};

struct derived : public base
{
    derived(){ cout << "Inside derived constructor" << endl; }
    void dfunc(void) { base::myfunc(); cout << "Inside dfunc" << endl; }
    virtual ~derived() { cout << "Inside ~derived" << endl; }

    protected:
    void myfunc(void) { cout << "derived myfunc" << endl; }
};

struct B
{
    int p;
    B() : p(0) {}
    B(int i) : p(i) { }
    int operator + (int i)
    {
        return p+i;
    }

};

int main(void)
{
    //derived d;
    //d.dfunc();
    derived d2;
    base &b2=d2;
    b2.myfunc();

    cout << "After dfunc" << endl;
    derived::derived();
    //b.bfunc();
    cout << "After const base &b = derived::derived();" << endl;

    int k=B::B(10)+10;
    cout << k << endl;

    return 0;
}

