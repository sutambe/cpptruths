// $Id$
/************************************************
 *
 * Author: Aniruddha s. Gokhale
 * Course: CS283, Vanderbilt University
 * Created: March 6, 2008
 *
 * Implements a unreliable channel that can play havoc
 * with the sending (and maybe receiving operations,
 * if we want to be adventurous)
 ************************************************/


#include "UnrelChannel.h"

// our unreliable channel uses the SOCK Dgram capabilities to send and
// receive requests. We call it unreliable since it can play havoc in
// terms of dropping packets, duplicating packets, corrupting packets.


// constructor
UnrelChannel::UnrelChannel (void)
  : ACE_SOCK_Dgram ()
{
}

// destructor
UnrelChannel::~UnrelChannel (void)
{
  // nothing to do
}

// we only override the send method since we want to play havoc in
// this method :-)
ssize_t UnrelChannel::send (const void *buf, 
                            size_t n, 
                            const ACE_Addr &addr, 
                            int flags) const
{
  /*************************** TO-DO ********************************/
  // generate a random number between x and y. Break the range into 4
  // parts. One part things get sent without any problem. In another you
  // send duplicates. In the third you refuse to send anything i.e.,
  // just drop the packet as if you never got it. In the last you
  // corrupt it.
  /******************************************************************/

  // I am only implementing the good behavior
  return this->ACE_SOCK_Dgram::send (buf, n, addr, flags);
}
 
  /*************************** TO-DO (optional) **************************/
// if you want to play mischief in the receive method too, then you
// must override this one. I am assuming the receive operations are all fine
// ssize_t 	recv (void *buf, size_t n, ACE_Addr &addr, int flags=0) const


