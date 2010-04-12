#include <iostream>

using namespace std;

class Trace 
{ 
	public: 
		Trace( const char *msg ) : msg_(msg) 
		{ std::cout <<  "Entering "   << msg_ << std::endl; } 
	
		~Trace() { std::cout <<  "Leaving "   << msg_ << std::endl; } 
	
	private: 
		std::string msg_; 
};

int main(void)
{
	Trace t1("t1");
	Trace t2("t2");
	Trace t3("t3");
	
	{
		Trace t4("t4");
		goto ABC;
		Trace t5("t5");
	}
ABC:
	
	return 0;
}
