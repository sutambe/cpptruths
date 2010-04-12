#include <iostream>
using namespace std;
namespace N {
    void g(int) { cout << "g(int)\n"; }
};

//What is the difference between these two statements?
// The output differs

//using namespace N; // Opens entire namespace. Hence future g will also be
//visible.
using N::g; // Brings existing g in the global namespace.

namespace N {
    void g(char){ cout << "g(char)\n"; }
};

int main(){
    g('x');
      return 0;
}

