// $Id$
/************************************************
 *
 * Author: Aniruddha s. Gokhale
 * Course: CS283, Vanderbilt University
 * Created: March 6, 2008
 *
 * Defines the interface for the base class for reliable
 * transfer. 
 *
 * Students should define their classes for the three protocols by
 * inheriting from this class.
 ************************************************/


#ifndef _CS283_REL_DATA_TRANSFER_SENDER_H_
#define _CS283_REL_DATA_TRANSFER_SENDER_H_

#include <string>
using namespace std;

#include <ace/INET_Addr.h>        // to maintain address
#include <ace/Timer_Queue.h>      // for timers
#include <ace/Event_Handler.h>    // for event handling
#include <ace/Reactor.h>          // for reactor functionality

// following files is needed for our unreliable channel
#include "UnrelChannel.h"



// we make the reliable transfer sender be an event handler 
class RelDataTransferSender : public ACE_Event_Handler
{
public:
  // constructor
  RelDataTransferSender (ACE_Reactor *r = ACE_Reactor::instance ());

  // destructor
  ~RelDataTransferSender (void);

  // initialization method
  virtual int open (const string &hostname,
                    unsigned short port_num,
                    size_t chunk_size);

  // this method must be supplied by the specialized handler class
  // It should return the demultiplexing handle
  virtual ACE_HANDLE get_handle (void) const;

  // we need to override those methods of the event handler class
  // which we are going to specialize for our case. In our case
  // at the minimum we are concerned with the incoming network data and
  // also timers

  // send data. This is the entry point to send the entire data as a
  // buffer. Return number of bytes sent on success and -1 on error.  
  virtual size_t  send_data (void *buffer, size_t length);

  // handle incoming events
  virtual int handle_input (ACE_HANDLE h = ACE_INVALID_HANDLE);

  // cleanup when handle is to be closed
  virtual int handle_close (ACE_HANDLE h = ACE_INVALID_HANDLE);

  // handle a timeout
  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *arg = 0);

  // some useful methods. We may want to use these when we want to
  // repeat an experiment for different chunk sizes.

  // mutator and accessor for chunk size.
  void set_chunk_size (size_t chunk_size);
  size_t get_chunk_size (void) const;

protected:
  // we make things protected so that our derived classes can access
  // them

  ACE_Reactor *reactor_;    // reactor (we do not own it)

  UnrelChannel  channel_;   // our unreliable channel

  size_t chunk_size_;       // chunk size

  ACE_INET_Addr server_addr_; // server address

private:
  // I am purposely making this timer variable a private class since
  // your protocols may need more than one timer particularly for the
  // pipelined approaches.

  // maintain the timer ID for the timer we will schedule
  long timer_id_;

  // data buffer (maintain the pointer to the data being sent)
  void  *data_;

  // total size of data to be sent
  size_t total_data_bytes_;

  // bytes sent thus far
  size_t bytes_sent_;

};

#endif  /* _CS283_REL_DATA_TRANSFER_SENDER_H_ */

