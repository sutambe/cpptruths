// $Id$

#include "ace/Timer_Queue.h"
#include "ace/Reactor.h"
// following files is needed for Dgram
#include "ace/SOCK_Dgram.h"


// Define our Protocol handler class. This must inherit from the ACE's Event 
// Handler class
class Proto_Handler : public ACE_Event_Handler 
{
public:

  // constructor
  Proto_Handler (void);

  // destructor
  ~Proto_Handler (void);

  // initialization method
  int open (int argc, ACE_TCHAR *argv[], 
            ACE_Reactor *r = ACE_Reactor::instance ());

  // this method must be supplied by the specialized handler class
  // It should return the demultiplexing handle
  virtual ACE_HANDLE get_handle (void) const;

  // we need to override those methods of the event handler class
  // which we are going to specialize for our case. In our case
  // at the minimum we are concerned with the incoming network data and
  // also timers

  // handle incoming events
  virtual int handle_input (ACE_HANDLE h = ACE_INVALID_HANDLE);

  // cleanup when handle is to be closed
  virtual int handle_close (ACE_HANDLE h = ACE_INVALID_HANDLE);

  // handle a timeout
  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *arg = 0);

protected:

  // we maintain the lower layer protocol object 

  ACE_SOCK_Dgram  dgram_;

  // maintain our reactor owner
  ACE_Reactor *reactor_;

  // maintain the timer ID for the timer we schedule
  long timer_id_;

};
