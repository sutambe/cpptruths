// $Id$
/************************************************
 *
 * Author: Aniruddha s. Gokhale
 * Course: CS283, Vanderbilt University
 * Created: March 6, 2008
 *
 * Defines the factory interface used to create the right set of
 * protocol object on the sending side.  Note that since we are
 * distinguishing the sending side protocol from receiver, we have to
 * have these two separate objects.
 ************************************************/


#ifndef _CS283_SENDER_PROTOCOL_FACTORY_H_
#define _CS283_SENDER_PROTOCOL_FACTORY_H_

// since we need the reactor, we include its header
#include "ace/Reactor.h"

// the header file for the sender-side protocol base class
#include "RelDataTransferSender.h"

class SenderProtocolFactory
{
public:

  // constructor
  SenderProtocolFactory (void);

  // destructor
  ~SenderProtocolFactory (void);

  // create the right set of protocol object. Note how the pointer we
  // pass out is that of the base class. At runtime polymorphism will
  // kick in an result in methods of your protocol class being invoked.
  //
  // notice that by creating a static method this method is invoked on
  // the class rather than on an object instance. We get some sense of a
  // global instance. There are more elegant ways of doing this using
  // ACE_Singleton but we will not use it here.
  static RelDataTransferSender 
     *create_protocol_object (const string &hostname,
                              unsigned short port_num,
                              unsigned short proto_type,
                              size_t chunk_size,
                              ACE_Reactor *r =
                              ACE_Reactor::instance ());
                                                  
};

#endif /* _CS283_SENDER_PROTOCOL_FACTORY_H_ */

