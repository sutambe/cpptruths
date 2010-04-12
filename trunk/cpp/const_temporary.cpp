#include <iostream>
using namespace std;

class scoper {
    string name_;
public:
    scoper(const string& name) : name_(name) { }
    ~scoper() { cout << "But I hardly even know " << name_ << endl; }
    string name() const { return name_; }
};

class holder {
    const scoper& scope_;
public:
    holder(const scoper& scope)
        : scope_(scope)
    {
        cout << "Holding on to " << scope_.name() << endl;
    }
    ~holder() {    cout << "Letting go of " << scope_.name() << endl;    }
};

int main()
{
    holder h(scoper("'Er")); // pass temporary to ctor
    cout << "What happens now?\n";
    // If temporary and a const reference to it are in the same scope then destructor of the temporary
    // is not invoked. Check GotW #88 and a reader's comment on it.
    // Check the order of constructor and destructors above. 
    // It results in a dangling reference. 
}
