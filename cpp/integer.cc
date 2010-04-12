#include <iostream>

using namespace std;

struct Integer
{
	int i;
	Integer(int _i);
	Integer operator->();
	void decrement();
	void print();
};

struct Even : public Integer
{
	Even(int i);
	Integer operator->();
};
struct Odd : public Integer
{
	Odd(int i);
	Integer operator->();
};
struct Zero: public Integer
{
	Zero(int i);
	Integer * operator->();
};

Integer *integerptr;
Zero *zeroptr;
Odd *oddptr;
Even *evenptr;

int main(void)
{
	Even e(20);	
	
	integerptr=&e;
	evenptr=&e;
	oddptr=integerptr;
	zeroptr=integerptr;
	
	evenptr->print();
	
	return 0;
}

Integer::Integer(int _i): i(_i) 
{}
void Integer::decrement()
{
	--i;
}
void Integer::print()
{
	cout << "Chain over." << endl;
}

Even::Even(int i) : Integer(i) 
{}
Integer * Even::operator->()
{
	if(i==0) return zeroptr;
	else if(i % 2) return oddptr;
	else 
	{
		cout << i << endl;
		Integer::decrement();
		return oddptr;
	}
}
Odd::Odd(int i) : Integer(i) 
{}

Integer * Odd::operator->()
{
	if(i==0) return zeroptr;
	else if((i % 2)==0) return evenptr;
	else 
	{
		cout << i << endl;
		Integer::decrement();
		return evenptr;
	}
}

Zero::Zero(int i) : Integer(i) 
{}

Integer * Zero::operator->()
{
	if(i != 0)
	{
		if(i % 2) return oddptr;
		else return evenptr;
	}
	cout << i << endl;
	return integerptr;
}
