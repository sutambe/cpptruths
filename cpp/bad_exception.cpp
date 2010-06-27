#include <iostream>
#include <exception>
#include <cstdlib>

void my_unexpected () {
	if (!std::uncaught_exception())
          std::cerr << "my_unexpected called\n";
        throw;
        
// NOTE: unexpected_handler can't return.
// if it returns, terminate handler will be called.
}
void my_terminate () {
	std::cerr << "my_terminate called\n";
}
void func (){
	std::cerr << "func called\n";
}
//void g () throw (std::bad_exception, int) 
void g () throw (std::exception, int) 
//void g () throw (int) 
// Seems like bad_exception must be used in conjunction with unexpected
// handler to really convert an unknown exception into bad_exception.
// That kind of makes sense, doesn't it?
{
	throw 1.0; // throws double
}

int main (void) {
  std::set_unexpected (my_unexpected);
  std::set_terminate (my_terminate);
  //std::set_terminate(__gnu_cxx::__verbose_terminate_handler);
  atexit (func);
  try {	
    g();
  }
  catch (int i)   {
    std::cerr << "caught int:" << i << "\n";
  }
  catch (std::bad_exception const & e)   {
    std::cerr << "bad_exception: " << e.what() << std::endl;
  }
  return 0;
}

