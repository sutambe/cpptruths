// $Id$
/************************************************
 *
 * Author: Aniruddha s. Gokhale
 * Course: CS283, Vanderbilt University
 * Created: March 6, 2008
 *
 * This class implements the file transfer client that will
 * provide reliable file transfer over an unreliable channel
 ************************************************/

#include <string>
using namespace std;

#include <ace/OS_NS_errno.h>      // error codes
#include <ace/OS_NS_string.h>     // for memset
#include <ace/Log_Msg.h>          // for error logging

// following files used for socket based code
#include <ace/OS_NS_unistd.h>

// following files is needed for Dgram
#include <ace/SOCK_Dgram.h>

// needed for timing measurements
#include <ace/Time_Value.h>

// needed for memory mapped I/O
#include <ace/Mem_Map.h>

// needed for the reactors
#include <ace/Select_Reactor.h>
#include <ace/Reactor.h>

// the factory on the sender side
#include "SenderProtocolFactory.h"

// our header for the file transfer object
#include "FileTransferClient.h"

// constructor
FileTransferClient::FileTransferClient (void)
  : proto_object_ (0),
    reactor_ (0)
{
}

// destructor
FileTransferClient::~FileTransferClient (void)
{
  // delete the reactor we created
  delete this->reactor_;
}

// file transfer session
int FileTransferClient::open (const string &hostname,
                              unsigned short port_num,
                              unsigned short proto_type,
                              size_t chunk_size)
{
  // if there is no reactor, create one
  if (!this->reactor_) {
    // now instantiate a reactor object.  In this sample, we show the
    // use of single threaded select based reactor 
    ACE_Select_Reactor  *select_reactor = new ACE_Select_Reactor;
  
    // tell ACE that we want to use a select reactor object.  There is a
    // default second argument to the constructor which tells the
    // reactor class whether it should take ownership of cleaning up the
    // implementation. In this assignment we are asking the newly
    // created reactor to take up the ownership.  Since we dynamically
    // create it, we let the reactor own the implementation and assume
    // ownership of cleaning it.
    this->reactor_ = new ACE_Reactor (select_reactor, true);

    // technically we should check if reactor is allocated or not
    if (!this->reactor_) {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("[%P] FileTransferClient::open - ")
                  ACE_TEXT ("failed to create reactor (%m)\n")));
      return -1;
    }
  }

  // now initialize the reliable transfer protocol object. We ask a
  // factory object to provide us with the right kind of protocol
  // object.  Notice how we invoke the static method on the class.
  this->proto_object_ =
    SenderProtocolFactory::create_protocol_object
    (hostname, port_num, proto_type, chunk_size, this->reactor_);

  // check if there is no error
  if (!this->proto_object_) {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("[%P] FileTransferClient::open - ")
                ACE_TEXT ("failed to create protocol object (%m)\n")));
    return -1;
  }

  // success
  return 0;
}

// Send a file
// Takes a file name as an argument. Since we cannot send arbitary
// sized data on an underlying UDP transport, we are going to break
// the file into chunk sizes. By default we will use a chunk size of
// 256 but the user can set a different size
//
// Returns -1 on error and bytes sent on success
size_t FileTransferClient::SendFile (const string &filename)
{

  ACE_Mem_Map mmap; // we choose to use memory mapped files so that we
                    // can access the file contents directly from a
                    // buffer 

  // now map the file into memory
  if (mmap.map (filename.c_str ()) == -1) {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("[%P] FileTransferClientClient::SendFile - ")
                ACE_TEXT ("failed to map file (%m)\n")));
    return -1;
  }

  // delegate sending of the the file to our reliable protocol handler
  return this->proto_object_->send_data (mmap.addr (),
                                         mmap.size ());
}

// say bye bye :-)
// Returns -1 on error and a 0 on success
int FileTransferClient::close (void)
{
  // the object was closed so we remove the underlying protocol object
  delete this->proto_object_;

  // success
  return 0;
}


// some useful methods

// mutator and accessor for chunk size. Will affect its protocol
// object 
void FileTransferClient::set_chunk_size (size_t chunk_size)
{
  // technically we should check if the underlying proto_object exists
  // or not. If not some exception needs to be thrown. I have not done
  // all of this defensive programming.
  this->proto_object_->set_chunk_size (chunk_size);
}

size_t FileTransferClient::get_chunk_size (void) const
{
  // technically we should check if the underlying proto_object exists
  // or not. If not some exception needs to be thrown. I have not done
  // all of this defensive programming.
  return this->proto_object_->get_chunk_size ();
}


