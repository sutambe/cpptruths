#include <iostream>
#include <exception>

void my_unexpected () {
	if (!std::uncaught_exception())
          std::cerr << "my_unexpected called\n";
        throw;
        
// NOTE: unexpected_handler can't return.
}
void my_terminate () {
	std::cerr << "my_terminate called\n";
}
void func (){
	std::cerr << "func called\n";
}
void g () throw (std::bad_exception, int) {
	throw 1.0; // throws double
}

int main (void) {
  std::set_unexpected (my_unexpected);
//  std::set_terminate (my_terminate);
  std::set_terminate(__gnu_cxx::__verbose_terminate_handler);
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

