// $Id$
/************************************************
 *
 * Author: Aniruddha s. Gokhale
 * Course: CS283, Vanderbilt University
 * Created: March 6, 2008
 *
 * This class defines a file transfer client that will 
 * provide reliable file transfer over an unreliable channel.
 ************************************************/

// note how we use these macros to prevent multiple inclusions of the
// definitions. 
#ifndef _CS283_FILE_TRANSFER_CLIENT_H_
#define _CS283_FILE_TRANSFER_CLIENT_H_

#include <string>
using namespace std;

// reactor
#include <ace/Reactor.h>

// we maintain the reliable transfer protocol object
#include "RelDataTransferSender.h"

// a class that the client program uses to kickstart the file transfer
// session 
class FileTransferClient
{
public:

  // constructor
  FileTransferClient (void);

  // destructor
  ~FileTransferClient (void);

  // file transfer session
  int open (const string &hostname,
            unsigned short port_num,
            unsigned short proto_type = 0,
            size_t chunk_size = 256);

  // Send a file
  // Takes a file name as an argument. Since we cannot send arbitary
  // sized data on an underlying UDP transport, we are going to break
  // the file into chunk sizes. By default we will use a chunk size of
  // 256 but the user can set a different size
  //
  // Returns -1 on error and number of bytes sent on success
  size_t SendFile (const string &filename);

  // say bye bye :-)
  // Returns -1 on error and a 0 on success
  int close (void);

  // some useful methods. We may want to use these when we want to
  // repeat an experiment for different chunk sizes alone but not change
  // anything else.

  // mutator and accessor for chunk size. Will affect its protocol
  // object 
  void set_chunk_size (size_t chunk_size);
  size_t get_chunk_size (void) const;

private:
  /*********** data members **************/

  // the underlying protocol object that we maintain. Notice how we use
  // the base class pointer. The real object will be allocated by a
  // factory. Polymorphism will play a role in deciding which method
  // gets called.
  RelDataTransferSender *proto_object_;

  // we maintain our copy of a reactor
  ACE_Reactor *reactor_;
};

#endif  /* _CS283_FILE_TRANSFER_CLIENT_H_ */
