// $Id$
/************************************************
 *
 * Author: Aniruddha s. Gokhale
 * Course: CS283, Vanderbilt University
 * Created: March 6, 2008
 *
 * Defines the interface for the base class for reliable
 * transfer. This one implements a simple stop and wait protocol i.e.,
 * sends a chunk and waits for ACK, then sends one more and so on until
 * everything is sent.
 *
 * Students should define their classes for the three protocols from
 * this class.
 ************************************************/

#include <iostream>
#include <string>
using namespace std;              // use the "std" namespace

#include "ace/OS_NS_errno.h"      // error codes
#include "ace/OS_NS_string.h"     // for memset
#include "ace/OS_NS_unistd.h"     // process id
#include "ace/Log_Msg.h"          // error logging

#include "RelDataTransferSender.h"


// constructor
RelDataTransferSender::RelDataTransferSender (ACE_Reactor *reactor)
  : reactor_ (reactor),
    channel_ (),
    chunk_size_ (256), // default
    server_addr_ (),
    /* the following are private */
    timer_id_ (-1),
    data_ (0),  // no data to send
    total_data_bytes_ (0),
    bytes_sent_ (0)
{
}

// destructor
RelDataTransferSender::~RelDataTransferSender (void)
{
}

// initialization method
int RelDataTransferSender::open (const string &hostname,
                                 unsigned short port_num,
                                 size_t chunk_size)
{
  // initialize the server side remote addr (port and IP addr interface)
  this->server_addr_.set (port_num, hostname.c_str ());

  // initialize chunk size
  this->chunk_size_ = chunk_size;

  // now initialize our side of the end point, which is a datagram
  if (this->channel_.open (ACE_Addr::sap_any, // send/recv over any interface
                           this->server_addr_.get_type ()) == -1) {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("[%P] RelDataTransferSender::open - ")
                ACE_TEXT ("failed to open local side (%m)\n")));
    return -1;
  }

  // technically we should check that the reactor is valid
  if (!this->reactor_) {
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t) %p\n"), 
                ACE_TEXT ("bad reactor")));
    return -1;
  }

  // now register ourselves for both the READ event and timeout event
  if (this->reactor_->register_handler (this, 
                                        ACE_Event_Handler::READ_MASK) == -1){
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("[%P] RelDataTransferSender::open - ")
                ACE_TEXT ("failed to register handler (%m)\n")));
    return -1;
  }

  // everything went well
  return 0;
}

// this method must be supplied by the specialized handler class
// It should return the demultiplexing handle
ACE_HANDLE RelDataTransferSender::get_handle (void) const
{
  // delegate it to our unreliable channel
  return this->channel_.get_handle ();
}

// send data. The assumption is that the application layer hands us the
// entire buffer and its length. We then break it up in chunks and send
// one by one.
size_t RelDataTransferSender::send_data (void *buffer,
                                         size_t length)
{
  // update our protocol state since we will need access to the buffer
  // and other variables
  this->data_ = buffer;
  this->total_data_bytes_ = length;
  this->bytes_sent_ = 0;

 
  // now send the first chunk and rely on the reactor to wake us up when
  // the ACK shows up. However, make sure we send less data if the
  // length is less then chunk size
  size_t sent_bytes = 
    this->channel_.send (this->data_,
                         // see how the ? operator is used
                         ((this->chunk_size_ // if length is
                           < length)         // less than chunk,
                          ? this->chunk_size_ // send length
                          : length),    // number of bytes
                         this->server_addr_);
  if (sent_bytes == -1) {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("[%P] RelTransferSender::send_data - ")
                ACE_TEXT ("failed to send first chunk (%m)\n")));
    return -1;
  } 

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("[%P] RelTransferSender::send_data - ")
              ACE_TEXT ("sent %d bytes\n"),
              sent_bytes));


  // advance our data pointer to a starting position from where the
  // next chunk should go out.
  this->bytes_sent_ += sent_bytes;

  // remember we must wait for an ACK for what we sent. We do this by
  // relying on the reactor to wake us up
  
  /********** for demo purpose only *********/
  // now let us start a timer of 5 seconds. If we do not receive anything
  // in this much time, the underlying reactor will throw a timeout.
  // See how we initialized the ACE_Time_Value instance with a value of
  // 5 sec.  Note that this is just for demonstration purpose and this
  // timer, when it goes off, is not going to have any impact on this
  // protocol.  

  ACE_Time_Value  tv (5);
  if ((this->timer_id_ 
       = this->reactor_->schedule_timer (this, // pass ourselves as a parameter
                                         0,    // no argument
                                         tv)) == -1) {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("[%P] RelDataTransferSender::send_data - ")
                ACE_TEXT ("failed to schedule timer (%m)\n")));
    return -1;
  }

  // run the famous :-) reactor event loop. We will show later how to
  // break the loop
  if (this->reactor_->run_reactor_event_loop () == -1) {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("[%P] RelTransferSender::send_data - ")
                ACE_TEXT ("failed to run reactor loop (%m)\n")));
    return -1;
  }

  // when we break the above loop we should have sent all the
  // information. Return how many bytes we sent in all. They better be
  // the same as total bytes !!

  // we are at this point indicates that we are as such done and must
  // not be any longer associated with the reactor. So do the following
  // cleanup. 
  // now unregister ourselves for the READ event and timeout event
  if (this->reactor_->remove_handler (this, 
                                      ACE_Event_Handler::READ_MASK) == -1){
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("[%P] RelDataTransferSender::send_data - ")
                ACE_TEXT ("failed to remove handler (%m)\n")));
    return -1;
  }

  // cancel timer. The default second argument is true, which tells the
  // reactor not to call our handle_close method on canceling the timer.
  if (this->reactor_->cancel_timer (this) == -1) {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("[%P] RelDataTransferSender::send_data - ")
                ACE_TEXT ("failed to cancel timer (%m)\n")));
    return -1;
  }

  // now actually send whatever the bytes we managed to send.
  return this->bytes_sent_;

}

// we need to override those methods of the event handler class
// which we are going to specialize for our case. In our case
// at the minimum we are concerned with the incoming network data and
// also timers.  Timeouts are ignored and are shown here only for
// demonstration purposes.

// handle incoming events
int RelDataTransferSender::handle_input (ACE_HANDLE h)
{
  // In our protocol we hope to receive an ACK from the server. Once we
  // receive that we move on to send the next chunk and the cycle
  // repeats until everything is sent.

  char recv_buff [128];
  ACE_OS::memset (recv_buff, 0, sizeof (recv_buff));

  ACE_INET_Addr peer;  // we need this dummy variable for the recv call.
  size_t recv_bytes =
    this->channel_.recv (recv_buff, 
                         sizeof (recv_buff), 
                         peer);

  if (recv_bytes == -1) {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("[%P] RelTransferSender::handle_input - ")
                ACE_TEXT ("failed on receive (%m)\n")));
    return -1;
  }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("[%P] RelTransferSender::handle_input - ")
              ACE_TEXT ("Received data: %s\n"),
              recv_buff));

  // how much is remaining?
  size_t remaining_bytes = this->total_data_bytes_ - this->bytes_sent_;

  if (remaining_bytes == 0) {
    // we have received an ACK and there is nothing more to send. This
    // is the time to break the reactor's event loop.
    return this->reactor_->end_reactor_event_loop ();
  }

  // now send the next chunk and rely on the reactor to wake us up when
  // the ACK shows up just like how we did it here. However, make sure
  // we send less data if the remaining length is less then chunk size.
  // Notice how the starting position of the data to be sent is
  // computed. Also note that we have to cast the void* to char*. Only
  // then can we perform pointer arithmetic. Recall our goal was to
  // reach the next starting position.

  size_t sent_bytes = 
    this->channel_.send (((char *)this->data_ + this->bytes_sent_), 
                         // see how the ? operator is used
                         ((this->chunk_size_ // if remaining bytes
                           < remaining_bytes) // is less than chunk 
                          ? this->chunk_size_ // send remaining
                          : remaining_bytes), // number of bytes
                         this->server_addr_);
  if (sent_bytes == -1) {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("[%P] RelTransferSender::handle_input - ")
                ACE_TEXT ("failed to send next chunk (%m)\n")));
    return -1;
  } 

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("[%P] RelTransferSender::handle_input - ")
              ACE_TEXT ("sent %d bytes\n"),
              sent_bytes));


  // advance our data pointer to the starting position from where the
  // next chunk should go out.
  this->bytes_sent_ += sent_bytes;

  // remember we must wait for an ACK for what we sent just like we did
  // before. 

  // now let us start timer of 5 seconds. If we do not receive anything
  // in this much time, the underlying reactor will throw a timeout.
  // See how we initialized the ACE_Time_Value instance with a value of
  // 5 sec.  Note that this is just for demonstration purpose and this
  // timer when it goes off is not going to have any impact on this
  // protocol.  

  ACE_Time_Value  tv (5);
  if ((this->timer_id_ 
       = this->reactor_->schedule_timer (this, // pass ourselves as a parameter
                                         0,    // no argument
                                         tv)) == -1) {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("[%P] RelDataTransferSender::send_data - ")
                ACE_TEXT ("failed to schedule timer (%m)\n")));
    return -1;
  }

  return 0;
}

// handle a timeout
int RelDataTransferSender::handle_timeout (const ACE_Time_Value &tv,
                                           const void *arg)
{

  // only for demonstration purpose. You will need to do things
  // differently in your handle timeout.
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("[%P] RelDataTransferSender::handle_timer - ")
              ACE_TEXT ("Received timeout - IGNORE\n")));

  // success
  return 0;
}

// cleanup when handle is to be closed
int RelDataTransferSender::handle_close (ACE_HANDLE h)
{
  return this->channel_.close ();
}


// some useful methods

// mutator and accessor for chunk size.
void RelDataTransferSender::set_chunk_size (size_t chunk_size)
{
  this->chunk_size_ = chunk_size;
}

size_t RelDataTransferSender::get_chunk_size (void) const
{
  return this->chunk_size_;
}

