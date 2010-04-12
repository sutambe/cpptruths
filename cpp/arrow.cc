#include <iostream>

using namespace std;

int i=100;

struct Downtozero
{
	Downtozero & operator ->()
	{
		if(i==0) exit(0);
		else { cout << --i; return *this; }
	}
	void print() {}

};

int main(void)
{
	Downtozero d;
	d->print();
	
}

