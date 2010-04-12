// $Id$

#include <iostream>
#include <string>
using namespace std;              // use the "std" namespace

#include "ace/OS_NS_errno.h"      // error codes
#include "ace/OS_NS_string.h"     // for memset
#include "ace/OS_NS_unistd.h"     // process id
#include "ace/Log_Msg.h"          // error logging

#include "proto_handler.h"

// constructor
Proto_Handler::Proto_Handler (void)
  : reactor_ (0),
    timer_id_ (-1)
{
}

// destructor
Proto_Handler::~Proto_Handler (void)
{
}

// initialization
int Proto_Handler::open (int argc, ACE_TCHAR *argv[], ACE_Reactor *r)
{
  // first declare the variables
  u_short port_num;               // port number is unsigned short
  ACE_INET_Addr  server_addr;     // address abstraction for us

  // you must first parse the command line arguments if any
  // **** ADD CODE HERE ***


  /* Now let us initialize the server */

  // We will use a simple scheme to get create a port number for us to use.
  // We will use the process ID and add 10,000 to it.
  //  port_num = 10000 + (u_short) ACE_OS::getpid ();
  port_num = 10000;

  cout << "Server will use port = " << port_num << endl;
  cout << "Server's hostname is " << server_addr.get_host_name () << endl;

  // initialize the server addr (listening on port and 
  // accept on any interface)
  server_addr.set (port_num);

  // initialize the server (we use IPv4 family)
  if (this->dgram_.open (server_addr, AF_INET) == -1) {
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t) %p\n"), 
                ACE_TEXT ("server open")));
    return -1;
  }

  // successfully started the server
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) started server at port %d\n"),
              server_addr.get_port_number ()));

  // set our reactor
  this->reactor_ = r;

  // technically we should check that the reactor is valid
  if (!this->reactor_) {
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t) %p\n"), 
                ACE_TEXT ("bad reactor")));
    return -1;
  }

  // now register ourselves with the reactor. We are interested only
  // in the incoming events so we use the READ mask.
  if (this->reactor_->register_handler (this, 
                                        ACE_Event_Handler::READ_MASK) == -1){
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t) %p\n"), 
                ACE_TEXT ("register handler")));
    return -1;
  }

  // now let us start an initial timer of 5 seconds. If we do not
  // receive anything in this much time, the underlying reactor will
  // throw a timeout.  See how we initialized the ACE_Time_Value
  // instance with a value of 5 sec.
  ACE_Time_Value  tv (5);
  if ((this->timer_id_ 
       = this->reactor_->schedule_timer (this, //                                                                  // pass ourselves as a parameter
                                         0,    // no argument
                                         tv)) == -1) {
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t) %p\n"), 
                ACE_TEXT ("open::schedule time")));
    return -1;
  }

  // success
  return 0;
}


// provide the get handle method
ACE_HANDLE Proto_Handler::get_handle (void) const
{
  // note that dgram_ is of type ACE_SOCK_Dgram, which inherits from
  // ACE_SOCK that has the get_handle method defined on it
  return this->dgram_.get_handle ();
}

// handle input
int Proto_Handler::handle_input (ACE_HANDLE h)
{
  char data_buff [1024];        // define a buffer to hold client data
  char ack [128];               // define a buffer for ACK
  ACE_INET_Addr  peer_addr;     // address of other entity

  // We receive data from our clients. So I need to cancel my timer and
  // start a new one after I handle the data

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t)%s\n"), 
              ACE_TEXT ("handle_input::canceling timer")));

  if (this->reactor_->cancel_timer (this->timer_id_) == -1) {
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t) %p\n"), 
                ACE_TEXT ("handle_input::cancel_timer")));
    return -1;
  }

  /* Note that here I just do one read and one send but you may have
     to do a read of more number of bytes in a loop here depending on
     the protocol you have. The reason to do this is because you may
     want to read all that has arrived on the interface in one shot of
     handle_input.

     In other words depending on the reliable transport you are
     developing, different strategies need to be plugged in.  One
     approach is for each of the stratgies to inherit from the
     Proto_Handler class.
  */

  // clear the contents
  ACE_OS::memset (data_buff, 0, sizeof (data_buff)); // clear its contents
  ssize_t recv_bytes = 
    this->dgram_.recv (data_buff, 
                       sizeof (data_buff), 
                       peer_addr, 
                       0);
  if (recv_bytes == -1)
    {
      ACE_ERROR((LM_ERROR,
                 ACE_TEXT("%p\n"),
                 ACE_TEXT("recv for UDP over IPv4 failed")));
      return -1;
    }

  cout << "Server: Received: #" << recv_bytes << " bytes" << endl;
  //  cout << data_buff << endl;

  // send an ACK
  ACE_OS::memset (ack, 0, sizeof (ack));
  sprintf (ack, "ACK");
  ssize_t send_bytes = 
    this->dgram_.send (ack, 
                       ACE_OS::strlen (ack), 
                       peer_addr, 
                       0);
  if (send_bytes == -1)
    {
      ACE_TCHAR hostname_string[100];
      peer_addr.addr_to_string (hostname_string, sizeof (hostname_string));
      ACE_ERROR((LM_ERROR,
                 ACE_TEXT("%p\n"),
                 ACE_TEXT("Server UDP send to %s failed"),
                 hostname_string));
      return -1;
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t)%s\n"), 
              ACE_TEXT ("handle_input::scheduling a new timer")));

  // now let us start another of 5 seconds. If we do not
  // receive anything in this much time, the underlying reactor will
  // throw a timeout
  ACE_Time_Value  tv (5);
  if ((this->timer_id_ 
       = this->reactor_->schedule_timer (this, //                                                                  // pass ourselves as a parameter
                                         0,    // no argument
                                         tv)) == -1) {
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t) %p\n"), 
                ACE_TEXT ("handle_input::schedule timer")));
    return -1;
  }

  // success
  return 0;
}


// handle close
int Proto_Handler::handle_close (ACE_HANDLE h)
{
  // close our server
  this->dgram_.close ();

  return 0;
}

// handle a timeout
int Proto_Handler::handle_timeout (const ACE_Time_Value &tv,
                                   const void *arg)
{

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t)%s\n"), 
              ACE_TEXT ("timer expired, scheduling a new one")));

  // now let us restart a new timer of 5 seconds. If we do not
  // receive anything in this much time, the underlying reactor will
  // throw a timeout
  //
  // Note that each of your implementations is going to take a certain
  // action depending on what happens on a timeout. What I showed
  // below is a dummy action of restarting the timer.

  ACE_Time_Value  new_tv (5);
  if ((this->timer_id_ = 
       this->reactor_->schedule_timer (this, // pass
                                       //ourselves as a 
                                      // parameter
                                       0,    // no argument
                                       new_tv)) == -1) {
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t) %p\n"), 
                ACE_TEXT ("handle_timeout::schedule time")));
    return -1;
  }


  // success
  return 0;
}



