//The template keyword as qualifier
/*
If the name of a member template specialization appears after a ., ->, or ::
operator, and that name has explicitly qualified template parameters, prefix
the member template name with the keyword template. The following example
demonstrates this use of the keyword template:
   */
#include <iostream>
using namespace std;

class X {
     public:
     template <int j> 
       struct S {
            void h() {
                                      cout << "member template's member function: " << j << endl;
             }
       };
     template <int i> void f() {
                           cout << "Primary: " << i << endl;
                                   }
};

template<> void X::f<20>() {
     cout << "Specialized, non-type argument = 20" << endl;
}

template<class T> void g(T* p) {
     p->template f<100>();
        p->template f<20>();
           typename T::template S<40> s; // use of scope operator on a member     template
                s.h();
}

int main()
{
     X temp;
     g(&temp);
}
