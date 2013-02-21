#include <cstdio>

struct B {
void foo () {
printf ("B::foo");
}
};

void foo () {
printf("::foo");
}

template <typename Base>
struct Derived : public Base {
void bar () { foo (); }
};

int main (void) {
Derived <B> d;
d.bar();

return 0;
}
