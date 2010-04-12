
template <class OnlyFriend>  
class Uninheritable 
{ 
  friend class OnlyFriend; 
  Uninheritable(void) {} 
};

class NotABase : virtual public Uninheritable<NotABase> 
{ 
	// WHATEVER 
};

class NotADerived: public NotABase 
{ 
	// THIS CLASS GENERATES A COMPILER ERROR 
}; 

int main (void)
{
	return 0;
}


