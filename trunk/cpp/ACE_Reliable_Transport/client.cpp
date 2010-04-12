// $Id$

#include <iostream>
#include <string>
using namespace std;              // use the "std" namespace

// for timing
#include <ace/High_Res_Timer.h>

// logging
#include "ace/Log_Msg.h"

// the file transfer object we use
#include "FileTransferClient.h"

// client main function
// uses a portable form of the "main" function along with its arguments
int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{ 

  // first declare the variables
  string server_host;             // name of the server
  unsigned short port_num;        // port number is unsigned short
  unsigned short proto_type = 0;  // 0 is our dummy implementation
  size_t  chunk_size = 256;       // chunk size (default = 256)

  FileTransferClient  ftpClient;  // instantiate an object for file
                                  // transfer
  string fname;                   // name of file to transfer

  // we require the user to pass 4 arguments to the executable in that order
  // (1) the server host name 
  // (2) server port number 
  // (3) protocol to use for reliable transfer (choices are 1, 2 or 3)
  // (4) chunk size to use (optional)
  if (argc < 4) {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Usage: client <server hostname> <server port num> ")
                ACE_TEXT ("<proto type: 1 = RDT, ")
                ACE_TEXT ("2 = Go Back N, 3 = Selective Repeat> ")
                ACE_TEXT ("[<chunk size, default = 256>]\n")));
    return -1;
  }

  // let us grab the arguments
  server_host = argv [1];

  // note we have to convert to a number 
  port_num = (unsigned short) atoi (argv[2]);  

  // note we have to convert to a number 
  proto_type = (unsigned short) atoi (argv[3]);  

  // check if the user supplied a chunk size (the 4th argument)
  if (argc == 5) {  
    chunk_size = (unsigned short) atoi (argv[4]);  
  }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("[%P] Client - ")
              ACE_TEXT ("Server = %s, port = %d, proto = %d, ")
              ACE_TEXT ("Chunk size = %d\n"),
              server_host.c_str (), port_num, proto_type, chunk_size));

  // ask the user to enter a file name
  cout << "Please enter name of the file to transfer: ";
  cin >> fname;

  /*********************** TO-DO *************************************/
  // I will suggest that you upgrade the FileTransferClient class so
  // that it can keep a record of time taken against the chunk size and
  // the protocol type used.
  //
  // Moreover the following can be inserted into a loop where the server
  // host & port num stays same but you can vary both the proto type as
  // well as chunk size and find out the time it takes to send the same
  // file. Choose a file that is sufficiently large to show us some
  // variations. 
  /*******************************************************************/

  // now initialize the File Transfer object
  if (ftpClient.open (server_host,
                      port_num,
                      proto_type,
                      chunk_size) == -1) {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("[%P] client main - ")
                ACE_TEXT ("failed to open the object (%m)\n")));
    return -1;
  }

  // now let us time the transfer
  ACE_High_Res_Timer  timer;

  // start timing
  timer.start ();
  
  size_t bytesTransferred;  // keeps track of how many bytes were
                            // transferred 

  if ((bytesTransferred = ftpClient.SendFile (fname)) == -1) {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("[%P] client main - ")
                ACE_TEXT ("failed to send file (%m)\n")));
    return -1;
  }
  
  // time the transfer and retrieve elapsed time.
  timer.stop ();
  ACE_Time_Value tv;
  timer.elapsed_time (tv);

  // since we are done, we close the ftp object so it can clean up its
  // underlying protocol object. 
  if (ftpClient.close () == -1) {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("[%P] client main - ")
                ACE_TEXT ("failed to close the object (%m)\n")));
    return -1;
  }
  
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("[%P] Client main - ")
              ACE_TEXT ("Total time for file transfer of %d bytes ")
              ACE_TEXT ("is %d milli seconds\n"),
              bytesTransferred, tv.msec ()));


  // everything went smoothly
  return 0;

}
