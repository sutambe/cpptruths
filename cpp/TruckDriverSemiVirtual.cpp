#include <iostream>
#include <string>

using namespace std;

class TruckDriverVirtualMembers
{
public:
	string Name(void) { return myName; }

protected:
	string myName;
	int    myAge;
	string myHomeAddress;
};

class TruckDriver : virtual public TruckDriverVirtualMembers
{
public:
	TruckDriver(string name, string ID)
	: myLicenceID(ID)
	{
		myName = name;
	}

	string ID(void)	  { return myLicenceID; }

private:
	string myLicenceID;
};


class JapaneseTruckDriver : public TruckDriver
{
public:
	JapaneseTruckDriver(string name, string ID)
	: TruckDriver(name, ID)
	{}
};

class FrenchTruckDriver : public TruckDriver
{
public:
	FrenchTruckDriver(string name, string ID)
	: TruckDriver("M(me). " + name, ID)
	{}
};

class AmericanTruckDriver : public TruckDriver
{
public:
	AmericanTruckDriver(string name, string ID)
	: TruckDriver("Good ol' " + name, ID)
	{}
};

class AustralianTruckDriver : public TruckDriver
{
public:
	AustralianTruckDriver(string name, string ID)
	: TruckDriver("Bloody " + name, ID)
	{}
};


class InternationalTruckDriver : public JapaneseTruckDriver
			       , public FrenchTruckDriver
			       , public AmericanTruckDriver
			       , public AustralianTruckDriver
{
public:
	InternationalTruckDriver(string name, string JapanID, string FranceID,
				 	      string USID, string AustraliaID)
	: JapaneseTruckDriver	(name, JapanID)
	, FrenchTruckDriver	(name, FranceID)
	, AmericanTruckDriver	(name, USID)
	, AustralianTruckDriver	(name, AustraliaID)
	{}
};

#define  SIZEOF(Class) cout << #Class ": " << sizeof(Class) << " bytes" << endl;

int main(void)
{
	SIZEOF(TruckDriver);
	SIZEOF(FrenchTruckDriver);
	SIZEOF(JapaneseTruckDriver);
	SIZEOF(AmericanTruckDriver);
	SIZEOF(AustralianTruckDriver);
	SIZEOF(InternationalTruckDriver);
}
			


