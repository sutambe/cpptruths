#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

typedef int intarray[20];
typedef intarray * intarray_ptr;

int main (int argc, char *(*argv)[10], char *env[])
{
/*
   std::ifstream in("circle.cc");
   std::istreambuf_iterator<char> begin(in), end;
   std::string str(begin, end);
*/
   int i;
   int & p=i; // is not allowed
   const int &  q=i; // is not allowed
   int b[10];
   int (*a)[10]=&b;
   int (&r)[10]=b; // a reference to a pointer to an array of 10 integers.
   intarray_ptr z;
   std::cout << typeid(a).name() << endl;
/*
   std::ifstream   input("circle.cc");
   std::ostringstream temp;
   temp << input.rdbuf();
   std::string str = temp.str();
   std::cout << str;
*/
   return 0;
}

/*
#include <iostream>

using namespace std;

struct Window
{
    int p;
    virtual void onResize(int i) { p=100*i; };
};

class Widget : public Window
{
    public:
        int w;
        explicit Widget(int i) : w(i) {}
        virtual void onResize(int i) 
        { 
            //static_cast<Window>(*this).onResize(i);
            //Window::onResize(i);
            w=2*p; 
        };
};

void dosomework(const Widget &w);
int main(void)
{
    //dosomework(15);
    //dosomework(Widget(15));
    //dosomework(static_cast<Widget>(15));

    Widget w(5);
    w.onResize(10);
    
    cout << w.w << endl;
    
    return 0;
}
void dosomework(const Widget &w)
{
    cout << w.w << endl;
}
*/
/*
#include <iostream>
#include <iomanip>

static int para=7;
void f(int x=para);

int main(void)
{
    //void f(int x=70);
    para=70;
    f();
    {
        void f(int x=700);
        f();
    }
    ::f();

}
void f(int x)
{
    std::cout << x << std::endl;
}
*/
/*

#include <iostream>

int main(void)
{
    int n=10;
    char *to="ABCD", *to_orig;
    to_orig=to;
    char *from="abcd";
    switch(n % 4)
    {
        case 0: do { *to++=*from++;
        case 3:      *to++=*from++;
        case 2:      *to++=*from++;  
        case 1:      *to++=*from++;
             } while(--n > 0);
    }

    std::printf("%s\n",to_orig);
    return 0;
}

*/
/*

#include <iostream>
#include <vector>

using namespace std;

char *c[];
int main(void)
{
    char *ch[];
}

*/
/*
#include <iostream>
#include <vector>

using namespace std;

template<class T>
void f(T x)
{
    x++;
    cout << "Inside f " << x << endl;
}
template<class T>
void g(T x)
{
	T a=x; // attempted copy
	a++;
	cout << "Inside g " << a << endl;
}
void f(int &p) {}
void f(int p) {}
int main(void)
{
    int x=100;	  // z is an int
	int &y=x; // y is a int&
	//f(x);     // no surprise
    cout << "Inside main " << x << endl;
    //f(y);     // SURPRISE!
    cout << "Inside main " << y << endl;
    //g(y); // SURPRISE!
    cout << "Inside main " << y << endl;

    vector<double> vd;
    vd.push_back(10.45);
    cout << vd.at(0) << endl;

    return 0;
}

*/
/*
#include <iostream>
#include <typeinfo>

using namespace std;

typedef void NullType;

template <class H, class T> struct Typelist {};

template <class TList, unsigned int index> struct TypeAt;
template <class Head, class Tail>
struct TypeAt<Typelist<Head, Tail>, 0>
{
     typedef Head Result;
};
template <class Head, class Tail, unsigned int i>
struct TypeAt<Typelist<Head, Tail>, i>
{
     typedef typename TypeAt<Tail, i - 1>::Result Result;
};

typedef Typelist<char, Typelist<signed char,
     Typelist<unsigned char, NullType> > > CharTypes;


class X
{
    public:
        X() : private_(1) { }
        int Value() { return private_; }
    private:
        template<class T>
        void f( const T& t )   {
            cout << typeid(t).name() << " specialisation" << endl;
        }
       int private_;
};
template<>
void X::f( const int &t ){
     cout << "Integer specialisation" << endl;
}

typedef int *IntPtr;
const IntPtr p=0;

int main(void)
{
    X abc;
    abc.f(100.23);
    abc.f(555);

    std::cout << typeid(TypeAt<CharTypes,2>).name() << std::endl;
    std::cout << typeid(TypeAt<CharTypes,1>).name() << std::endl;
    std::cout << typeid(TypeAt<CharTypes,0>).name() << std::endl;

    return 0;
}
*/
/*
#include <stdio.h>
#include <string.h>

char *(*p)(const char *)=strdup;
char *(*q)(const char *)=&strdup;
char *(&r)(const char *)=strdup;
char *(&s)(const char *)=&strdup;
char *(*const&t)(const char *)=strdup;
char *(*const&u)(const char *)=&strdup;

void main(void)
{
    char a;
    const char *s1=&a;
    char *b[10];
    char **x=b;
PQR:
    const char **s2=x;
    printf("%p %p %p %p",p,strdup,&strdup,&&PQR);
    if(strdup == &strdup)
    { }


    //return 0;
}
*/

/*
#include <stdio.h>

typedef unsigned char       BYTE;

union DoubleBytes
{
	double whole;
	BYTE   bytes[8];
	DoubleBytes(double x){whole = x;}
	DoubleBytes(){whole = 0;}
	operator double (){return whole;}
};

    const char *buf = "showers";

    void h( const char **q )
        { // *q = buf;
        }

    int main()
        {
       char *p;

       h( &p );
       printf( "%s\n", buf );
       p[0] = 'f';   p[1] = 'l';
       printf( "%s\n", buf );
       return 0;
       }
*/
/*
#include <iostream>

struct A
{

};
extern "C++" {
    void foo(void);
}
int main(void)
{
    foo();

    return 0;
}
void foo(void)
{
    A*a=new A;
}
*/

/*
#include <iostream>
#include <iomanip>

using namespace std;

template<class T> class A
{
private:
	T _a;
public:
	A(void) {}
	void set (const T a) { _a = a; }
};

int main(void)
{
	A<int*> a;
	const int *x = new int(1);
	a.set(x);
}
*/

