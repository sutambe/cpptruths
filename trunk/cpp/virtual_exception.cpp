#include <stdexcept>
#include <cstdio>
#include <string>

struct MsgException : virtual std::exception
{ };

struct MsgRuntimeError : virtual MsgException, virtual std::runtime_error
{
  MsgRuntimeError(std::string s) : std::runtime_error(s) {}
};

int main(void)
{
  try {
    throw MsgRuntimeError("MsgRuntimeError");
  }
  catch(std::exception & e) {
    printf("std::exception = %s.\n", e.what());
  }
  catch(...) {
    printf("opps...\n"); // This is printed!!! Opps!!
  }
}

