//
// g++ thread.cpp -o thread -Wall -std=c++0x -lpthread
//
#include <iostream>
#include <thread>
using namespace std;

void my_thread_func()
{
  std::cout << "hello" << std::endl;
}

int main()
{
  thread t(my_thread_func);
  t.join();
}
