#include "ace/Reactor.h"
#include "ace/Event_Handler.h"
#include "ace/Time_Value.h"
#include <iostream>
#include "ace/Containers_T.h"

#include <sys/types.h>
#include <sys/socket.h>



class My_Handler : public ACE_Event_Handler
{
public:
  My_Handler() : mask_(0)  {
    std::cerr << "my_handler = " << this << std::endl;
    this->register_for_io();
    this->register_for_timer();
  }
  virtual ~My_Handler() {
    std::cerr << "Farewell cruel world.  Love, " << this << std::endl;}

  void register_for_io() {
    std::cerr << "registering for read_mask" << std::endl;    
    if (ACE_Reactor::instance()->register_handler(
            this, ACE_Event_Handler::READ_MASK) == -1)
    {
      std::cerr << "registering for READ_MASK failed" << std::endl;
      exit(1);
    }
    mask_ |= ACE_Event_Handler::READ_MASK;
    
    std::cerr << "registering for write_mask" << std::endl;    
    if (ACE_Reactor::instance()->register_handler(
            this, ACE_Event_Handler::WRITE_MASK) == -1)
    {
      std::cerr << "registering for WRITE_MASK failed" << std::endl;
      exit(1);
    }
    mask_ |= ACE_Event_Handler::WRITE_MASK;

  }

  void register_for_timer() {
    std::cerr << "scheduling timer" << std::endl;
    if (ACE_Reactor::instance()->schedule_timer(
            this, 0, ACE_Time_Value(0, 900000), ACE_Time_Value (0,1000000)) == -1)  {
      std::cerr << "schedule timer failed" << std::endl;
      exit(1);
    }
    mask_ |= ACE_Event_Handler::TIMER_MASK;
  }
  

  virtual int handle_timeout (const ACE_Time_Value &, const void *)  {
    std::cerr << "BEGIN handle_timeout" << std::endl;

//    std::cerr << " closing stdin " << close(ACE_STDIN) << std::endl;
//    std::cerr << " closing stdout " << close(ACE_STDOUT) << std::endl;

    std::cerr << "END returning from handle_timeout" << std::endl;
    return -1;
  }

  virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask close_mask) {
    std::cerr << "handle close.  My mask is " << mask_ << ", close mask is " << close_mask << std::endl;
    mask_ &= ~close_mask;
    std::cerr << "now my mask is " << mask_ << std::endl;

    if (mask_ == 0) {
      std::cerr << "delete this.... (if you see this message)" << std::endl;
      delete this;
    }
    return 0;
  }
 
  virtual int handle_input (ACE_HANDLE h) {
    char buf[4096];
    ssize_t s = recv(h, buf, 1, 0);

    if (s <= 0){
      std::cerr << s << " unregistering, cancel wakeup for write mask ... "
                << std::endl;

      reactor()->cancel_wakeup(this, ACE_Event_Handler::WRITE_MASK);      

      return -1;
    }

    std::cerr << "handle_input was called!!!!" << std::endl;  return 0;
  }
  virtual int handle_output (ACE_HANDLE)
  { 
    //std::cerr << "handle_output was called!!!!" << std::endl;  
    return 0; 
  }

  virtual ACE_HANDLE get_handle (void) const
  { 
    static int handle = 0;
    std::cerr << "get_handle() got called: " << handle
              << std::endl; 
    return handle;  
  }

private:
  ACE_Reactor_Mask mask_;  
};

class X
{
  public:
    X (const X &) {}
  private:
    X & operator = (X const &);
};

int main()
{
  std::cerr << "creating handler" << std::endl;
  new My_Handler;

  ACE_Time_Value duration(3);
  
  while (duration > ACE_Time_Value::zero)
  {
    //std::cerr << "calling Reactor::handle_events, duration is now "  << duration.usec() << std::endl;
    ACE_Reactor::instance()->handle_events(&duration);
  }
  std::cerr << "DONE calling Reactor::handle_events, duration is now "
            << duration.usec() << "Exiting..." << std::endl;
  ACE_Unbounded_Stack <X> stack;
}
/*
all: ace_test

ace_test: ace_test.cpp 
	g++ ace_test.cpp -o ace_test -Wall -I $(ACE_ROOT) -L $(ACE_ROOT)/lib -lACE

clean:
	rm -f ace_test
*/

