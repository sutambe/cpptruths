#include "ace/Reactor.h"
#include "ace/Event_Handler.h"
#include "ace/Time_Value.h"
#include "ace/SOCK_Connector.h"
#include "ace/Synch_Options.h"
#include "ace/Connector.h"
#include "ace/Svc_Handler.h"
#include "ace/INET_Addr.h"

#include <sys/types.h>
#include <sys/socket.h>

#include <vector>
#include <iostream>

class My_Handler 
  : public ACE_Svc_Handler <ACE_SOCK_Stream, ACE_NULL_SYNCH>
{
public:
  typedef ACE_Svc_Handler <ACE_SOCK_Stream, ACE_NULL_SYNCH> Super;

  My_Handler(int current = 0) 
    : current_ (current),
      mask_ (0)
  {
    std::cerr << "My_Handler = " << this << std::endl;
  }
  
  virtual ~My_Handler() 
  {
    std::cerr << "Farewell cruel world.  Love, " << this << std::endl;
  }
  
  virtual int handle_timeout (const ACE_Time_Value &, const void *)  
  {
    std::cout << "handle_timeout\n";
       // peer().close();
    return -1;
  }

  virtual int handle_close (ACE_HANDLE h, ACE_Reactor_Mask close_mask) 
  {
    //My_Handler::create (reactor(), current_ + 1);
    return Super::handle_close (h, close_mask);
  }

  virtual int open (void * factory)
  {
    this->register_for_timer();
    return Super::open (factory);
  }
  
  void register_for_timer() 
  {
      std::cerr << "scheduling timer" << std::endl;
      if (ACE_Reactor::instance()->schedule_timer(this, 0, 
            ACE_Time_Value(0, 5000000), ACE_Time_Value (0,1000000)) == -1)  
      {
        std::cerr << "schedule timer failed" << std::endl;
        exit(1);
      }
      mask_ |= ACE_Event_Handler::TIMER_MASK;
  }

  virtual int handle_input (ACE_HANDLE h) 
  {
    char buf[4096];
    ssize_t s = recv(h, buf, 4095, 0);
    if (s == 0)
    {
      return -1;
    }
    buf[s] = 0;
    std::cerr << buf;
    std::cerr.flush();

    return 0;
  }
/*
  static int create (ACE_Reactor * reactor, size_t current)
{
  static ACE_Connector <My_Handler, ACE_SOCK_Connector> 
     connector (reactor);
  
  std::vector <std::pair <std::string, size_t> > senders;
  senders.push_back (std::make_pair ("abcd.net", 35000));
  senders.push_back (std::make_pair ("localhost", 35000));
  senders.push_back (std::make_pair ("pqrs.net", 35000));

  if (senders.size() > current)
  {
    My_Handler *handler_ptr = new My_Handler (current);

    ACE_SOCK_Connector::PEER_ADDR serv_addr;
    serv_addr.set (senders[current].second, senders[current].first.c_str());
    ACE_Synch_Options options (ACE_Synch_Options::USE_TIMEOUT | 
                               ACE_Synch_Options::USE_REACTOR, ACE_Time_Value(5));

    if (connector.connect (handler_ptr, serv_addr, options) == -1
        && errno != EWOULDBLOCK)
    {
        std::cerr << "errno != EWOULDBLOCK\n";
        std::cerr
          << "Failed to open a connector to a RedBox snapshot sender @"
          << senders[current].first
          << ":"
          << senders[current].second
          << std::endl;
        /// @TODO Try repeatedly until some fixed number of tries.
    }
    else
       std::cerr << "errno == EWOULDBLOCK\n";
    
    return 0;
  }
  else
    return -1;
}
*/
private:
  size_t current_;
  unsigned int mask_;
};


int main(int argc, char *argv[])
{
  ACE_Connector <My_Handler,ACE_SOCK_Connector> connector;
  My_Handler * handler = 0;
  int port = atoi (argv[1]);
  ACE_SOCK_Connector::PEER_ADDR addr (port, "localhost");
  connector.connect(handler, addr);
  ACE_Reactor::instance()->run_reactor_event_loop();

  return 0;
}
/*
all: mobile_consumer

mobile_consumer: mobile_consumer.cpp 
	g++ mobile_consumer.cpp -o mobile_consumer -Wall -I $(ACE_ROOT) -L $(ACE_ROOT)/lib -lACE

clean:
	rm -f mobile_consumer
*/

