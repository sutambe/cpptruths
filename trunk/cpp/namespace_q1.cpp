
#include <iostream>
using namespace std;

namespace N {
    void g(int) {
          cout << "g(int)\n";
            }
};

using N::g;

void r() {
    g('x');
}

namespace N {
    void g(char){
          cout << "g(char)\n";
            }
};

//The following using declaration is the key.
//Commenting or uncommenting it makes difference in the output.
//using N::g;

void s() {
    g('x');
}

using namespace N;

void t() {
    g('x');
}

int main(){
    r();
      s();
        t();
          return 0;
}
/*
http://www.adtmag.com/joop/carticle.aspx?ID=851

   The answer hinges on which declarations of g are in scope at each call to g:
the version that takes an int, or the (better match) version that takes a
char.

Function r: The using-declaration "using N::g;" only introduces those
declarations of g that are in namespace N and are in scope at that point of
the using-declaration. This means that only the int version of g is introduced
by the using-declaration. Therefore, r calls g(int).

Function s: While g(char) has been declared at this point, it has been
declared in namespace N, and s is in the global namespace. No directives have
introduced N::g(char) into the global namespace at this point. Therefore s
calls g(int).

Function t: The using-directive "using namespace N;" introduces all the names
of namespace N from that point onward, so both versions of g are in scope.
Since g(char) is a better match than g(int), t calls g(char). So our program
prints:


g(int)
  g(int)
  g(char)
*/  
