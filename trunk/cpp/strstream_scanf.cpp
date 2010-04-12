#include <iostream>
#include <strstream>
#include <string>

using namespace std;

class eat {
public:
    eat (const char *);		// eat until string
    ~eat ();
    friend istream& operator>> (istream&, const eat&);
private:
    char * myString;
};

eat::eat (const char * s)
{
    myString = new char[strlen(s)+1];
    strcpy (myString, s);
}

eat::~eat ()
{
    delete [] myString;
}

istream& operator>> (istream& is, const eat& e)
{
    int n = 0;
    int len = strlen(e.myString);
    while (n < len && is) {
	char ch;
	is.get(ch);
	if (ch == e.myString[n]) {
	    n++;
	} else {
	    n = 0;
	}
    }
    return is;
}

int  main (void)
{
    int x, y;
    istrstream ss ("x=123 y=246");

    ss >> eat("x=") >> x >> eat("y=") >> y;
    //ss >> x;

    cout << "x is " << x << endl;
    cout << "y is " << y << endl;
    //cin >> x;
    return 0;
}
