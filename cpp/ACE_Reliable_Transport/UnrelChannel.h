// $Id$
/************************************************
 *
 * Author: Aniruddha s. Gokhale
 * Course: CS283, Vanderbilt University
 * Created: March 6, 2008
 *
 * Defines an unreliable channel that can play havoc
 * with the sending (and maybe receiving operations,
 * if we want to be adventurous)
 ************************************************/


#ifndef _CS283_UNREL_CHANNEL_H_
#define _CS283_UNREL_CHANNEL_H_

// since we inherit from SOCK_Dgram we need its include file
#include "ace/SOCK_Dgram.h"

// our unreliable channel uses the SOCK Dgram capabilities to send and
// receive requests. We call it unreliable since it can play havoc in
// terms of dropping packets, duplicating packets, corrupting packets.
class UnrelChannel : public ACE_SOCK_Dgram 
{
public:
  // constructor
 	UnrelChannel (void);

  // destructor
 	~UnrelChannel (void);

  // we only override the send method since we want it to play havoc  :-)
  ssize_t send (const void *buf, 
                size_t n, 
                const ACE_Addr &addr, 
                int flags = 0) const;
 
  // if you want to play mischief in the receive method too, then you
  // are free to override this one. For now I am not overriding it
  // ssize_t 	recv (void *buf, size_t n, ACE_Addr &addr, int flags=0) const

};

#endif /* _CS283_UNREL_CHANNEL_H_ */

