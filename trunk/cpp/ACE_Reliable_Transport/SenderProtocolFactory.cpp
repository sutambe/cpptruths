// $Id$
/************************************************
 *
 * Author: Aniruddha s. Gokhale
 * Course: CS283, Vanderbilt University
 * Created: March 6, 2008
 *
 * Implements the factory interface used to create the right set of
 * protocol object on the sending side.  Note that since we are
 * distinguishing the sending side protocol from receiver, we have to
 * have these two separate objects.
 ************************************************/


// the header file for the sender-side protocol base class
#include "SenderProtocolFactory.h"

/************ TO-DO ******************/
// It is here that you include all the three header files for the 3
// protocol classes you develop. The factory method should create the
// right object based on the proto_type parameter.
/*************************************/

// constructor
SenderProtocolFactory::SenderProtocolFactory (void)
{
  // nothing to do
}

// destructor
SenderProtocolFactory::~SenderProtocolFactory (void)
{
  // nothing to do
}

// create the right set of protocol object. Note how the pointer we
// pass out is that of the base class. At runtime polymorphism will
// kick in an result in methods of your protocol class being invoked.
//
// notice that by creating a static method this method is invoked on
// the class rather than on an object instance. We get some sense of a
// global instance. There are more elegant ways of doing this using
// ACE_Singleton but we will not use it here.
RelDataTransferSender *
SenderProtocolFactory::create_protocol_object 
   (const string &hostname,
    unsigned short port_num,
    unsigned short proto_type,
    size_t chunk_size,
    ACE_Reactor *reactor)
{
  // initialize the object you want to return to 0 (for defensive
  // programming) 
  RelDataTransferSender *proto_object = 0; 

  switch (proto_type) {
  case 0:
    // the base class which I have implemented to just pass data along
    proto_object = new RelDataTransferSender (reactor);
    break;
  case 1:
    /******* TO-DO ***********/
    // create a RDT_3_0 sender object

    /******* TO-DO ***********/
    // not implemented. Remove this when you implement it.
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("[%P] SenderProtocolFactory::create - ")
                ACE_TEXT (" protocol type RDT3.0 currently not implemented\n")));
    break;
  case 2:
    /******* TO-DO ***********/
    // create a GoBackN sender object

    /******* TO-DO ***********/
    // not implemented. Remove this when you implement it.
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("[%P] SenderProtocolFactory::create - ")
                ACE_TEXT (" protocol type GoBackN currently not implemented\n")));
    break;
  case 3:
    /******* TO-DO ***********/
    // create a SelectiveRepeat sender object

    /******* TO-DO ***********/
    // not implemented. Remove this when you implement it.
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("[%P] SenderProtocolFactory::create - ")
                ACE_TEXT (" protocol type SelectiveRepeat currently not implemented\n")));
    break;
  default:
    // bad choice
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("[%P] SenderProtocolFactory::create - ")
                ACE_TEXT (" Bad choice for protocol type\n")));
    return 0;
  }

  // having created the object we must now initialize it. Do so only if
  // the object exists. To keep the caller from having to do this we let
  // the factory object take up ownership of making sure the protocol
  // object is all initialized before passing it back to the caller.
  if (proto_object) {
    // now initialize the protocol object
    if (proto_object->open (hostname,
                            port_num,
                            chunk_size) == -1)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("[%P] SenderProtocolFactory::create - ")
                    ACE_TEXT (" initialize object failed\n")));
        // something went wrong. We better cleanup.
        delete proto_object;
        return 0;
      }
  } else {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("[%P] SenderProtocolFactory::create - ")
                ACE_TEXT (" NULL protocol object\n")));
    return 0;
  }

  // now return the object. Could be 0. Caller better check the return
  // value before using it else you will end up in trouble :-)
  //
  // Also remember that this factory does not own this allocated object
  // and hence the caller is responsible for deallocating it.
  return proto_object;
}


