// $Id$

#include "ace/ACE_export.h"

#include <iostream>
#include <string>
using namespace std;              // use the "std" namespace

#include "proto_handler.h"

// server main function
// uses a portable form of the "main" function along with its arguments
int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{ 

  // instantiate an object of the Proto Handler class
  Proto_Handler proto;

  // retrieve a reactor. Here we could have retrieved different
  // implementations of reactor. For now we get the default singletom
  // reactor. 
  ACE_Reactor *reactor = ACE_Reactor::instance ();

  // initialize the proto handler object
  if (proto.open (argc, argv, reactor) == -1) {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t) %p\n"), 
                  ACE_TEXT ("handler open")));
      return -1;
  }

  // now let the server handle the events
  for (;;) {
    if (reactor->handle_events () == -1) {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t) %p\n"), 
                  ACE_TEXT ("handle events")));
      return -1;
    }
  }

  // return exit status
  return 0;
}
