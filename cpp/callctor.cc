#include <stdio.h>

class A {
public:
  A() {
     printf("A::A (%p)\n",this);
  }
  explicit A(int i) {
     printf("A::A (%d) (%p)\n",i,this);
  }
  ~A() {
     printf("A::~A (%p)\n",this);
  }
};

extern "C" {
  void _ZN1AC1Ei(A *t, int i);
}

class B : A {
public:
  B(int i) {
     printf("B::B (%d) (%p)\n",i,this);
     _ZN1AC1Ei(this,7);  // comment this out and
     //A::A(7);          // uncomment this to see the temp created
  }
  ~B() {
     printf("B::~B (%p)\n",this);
  }
};

main()
{
   A(3); B(5);
   /*B *b; A *a;
   a = new A(3);
   b = new B(5);
   delete b;
   delete a;*/
}
