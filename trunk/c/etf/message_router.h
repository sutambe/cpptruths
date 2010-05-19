#ifndef __MESSAGE_ROUTER_H
#define __MESSAGE_ROUTER_H

#include "common.h"
#include "registrar.h"
#define __USE_UNIX98 /* for pthread_mutexattr_settype */
#include <pthread.h>

/**
    \brief An element of the list representing the tool bindings.

    This structure makes up a list of tool bindings. When an input pattern name of a 
    message pattern matches to that of the output pattern of some registered pattern
    a list of matching input `interfaces` (\e toolid and \e pattern_id) is populated.
 */
typedef struct InputInterface_tag
{
    Glist_element __g; //!< For the use of the generic list.
    int toolid;        //!< Represents the tool id.
    int pattern_id;    //!< Represents the pattern id of the above tool.
} InputInterface;

/**
    \brief Maintains bindings of all the patterns of a tool.

    The Router populates this structure for every tool which defines at least one
    output pattern. The structure maintains a list of tools interested in the messages
    corresponding to each registered pattern.
*/
typedef struct PatternBindings_tag
{
    GLIST(InputInterface) pattern_bindings[MAX_PATTERNS]; //!< An array of list of tools interested in every pattern
    int pattern_count; //!< Number of patterns, also the valid entries in the above array.
} PatternBindings;

struct Registrar_tag; /* Forward declaration */
struct MessagePattern_tag; /* Forward declaration */

/**
    \brief An element of a list of tools which sent a synchronous request.

    The Router populates this structure when a tool sends a synchronous request. The Router
    matches the \e request-reply id of the corresponding reply and delivers the reply to the
    tool identified by the \e toolid. 
 */
typedef struct RequestTool_tag
{
    Glist_element __g; //!< For the use of the generic list.
    int toolid;        //!< Indentifies the tool which made the request.
    int reqrepid;      //!< The request-reply id of the asynchronous request made by the above tool.
} RequestTool;

/**
    \brief Represents an instance of the Router.

    The Router delivers messages to the intended receivers based on the bindings it computes 
    using the patterns registered with the Registrar.
 */
typedef struct Router_tag
{
    PatternBindings comm_bindings[MAX_TOOLS]; //!< Maintains tool bindings of all the tools.
    pthread_mutexattr_t bindings_mutex_attr; //!< \e bindings_mutex attributes .
    pthread_mutex_t bindings_mutex;          //!< A mutex to serialize the access to the Router data structure.
    MessageQ mqueue; //!< The Router maintains a message queue.
    GLIST(RequestTool) request_tool_list; //!< A list of tools which have sent a synchronous request.
    struct Registrar_tag *registrar_ptr; //!< A pointer to the Registrar for quick access.
    unsigned int message_counter;        //!< Message counter to generate unique message ids.
    unsigned int request_reply_counter;  //!< request-reply counter to generate unique request-reply ids.
} Router;

//! Initializes a router.
/*! Initializes a router.
\param router An instance of the Router to initialize.
\return Nothing.
\sa router_deinit()
 */
void router_init(Router *router);

//! Deinitializes a router.
/*! Deinitializes a router.
\param router An instance of the Router to be destroyed.
\return Nothing.
\sa router_init()
 */
void router_deinit(Router *router);

//! Removes all the computed communication bindings between tools.
/*! This function erases all the communication bindings computed previously by the Router.
    IT does not change any registered pattern with the Registrar.
\param router An instance of the Router 
\return Nothing.
\sa compute_commbindings()
 */
void flush_commbindings(Router *router);

//! Computes the bindings between tools and populates the comm_bindings array.
/*! It uses the Registrar's internal data structures to compute the tool bindings
    A listener tool gets bound to an informer tool when the input pattern name of 
    the listener tool matches to that of the output patern of the informer tool.
\param router An instance of the Router 
\param registrar An instance of the Registrar
\return Returns zero on success.
\sa flush_commbindings()
 */
int compute_commbindings(Router *router, struct Registrar_tag *registrar);

//! Validates whether the tool indeed has a pattern registered having id \e pattern_id.
/*! Validates whether the tool indeed has a pattern registered having id \e pattern_id.
\param router An instance of the Router 
\param toolid The sender tool id.
\param pattern_id The pattern id.
\return Returns zero on success, -1 if the tool does not have a pattern registered of id \e pattern_id.
\sa new_notification(), new_request(), new_reply()
 */
int validate_para(Router *router, int toolid, int pattern_id);

//! Populates the message structure with the given parameters.
/*! It does not dynamically allocate memory for a new message, instead it 
    populates the message pointed by \e message. It assigns a unique message
    id to the message. This function is used internally.
\param router An instance of the Router 
\param message A pointer to the message to be populated
\param toolid The sender tool id.
\param st The status of the message (SUCCESS/FIALURE)
\param mode The delivery mode of the message. (SYNC_MODE, ASYNC_MODE)
\param pattern_id The pattern id corresponding to the message.
\param fptr A pointer to a function to destroy the message once it is processed
\return Returns zero on success, -1 on some error
\sa new_notification(), new_request(), new_reply()
 */
int new_message(Router *router, Message *message, int toolid, MessageStatus st, 
                       MessageMode mode, int pattern_id, MessageDestroy_fptr fptr);
                       
//! Converts a message pointed by \e message to a notification and populates the other fields of the structure. 
/*! It does not dynamically allocate memory for a new message, instead it 
    populates the message pointed by \e message. It assigns a unique message
    id to the message. It also sets the type of message to be a notification.
\param router An instance of the Router 
\param message A pointer to the message to be populated
\param toolid The sender tool id.
\param st The status of the message (SUCCESS/FIALURE)
\param mode The delivery mode of the message. (SYNC_MODE, ASYNC_MODE)
\param pattern_id The pattern id corresponding to the message.
\param fptr A pointer to a function to destroy the message once it is processed
\return Returns zero on success, -1 on some error
\sa new_message(), new_request(), new_reply()
 */
int new_notification(Router *router, Message *message, int toolid, MessageStatus st, 
                     MessageMode mode, int pattern_id, MessageDestroy_fptr fptr);

//! Converts a message pointed by \e message to a request and populates the other fields of the structure. 
/*! It does not dynamically allocate memory for a new message, instead it 
    populates the message pointed by \e message. It assigns a unique message
    id as well as a unique request-reply id to the message. It also sets the 
    type of the message to be a reqeust. 
\param router An instance of the Router 
\param message A pointer to the message to be populated
\param toolid The sender tool id.
\param st The status of the message (SUCCESS/FIALURE)
\param mode The delivery mode of the message. (SYNC_MODE, ASYNC_MODE)
\param pattern_id The pattern id corresponding to the message.
\param fptr A pointer to a function to destroy the message once it is processed
\return Returns zero on success, -1 on some error
\sa new_message(), new_notification(), new_reply()
 */
int new_request(Router *router, Message *message, int toolid, MessageStatus st, 
                MessageMode mode, int pattern_id, MessageDestroy_fptr fptr);
//! Converts a message pointed by \e message to a reply and populates the other fields of the structure. 
/*! It does not dynamically allocate memory for a new message, instead it 
    populates the message pointed by \e message. It assigns a unique message
    id to the message. It also sets the type of the message to be a reply. 
\param router An instance of the Router 
\param message A pointer to the message to be populated
\param toolid The sender tool id.
\param st The status of the message (SUCCESS/FIALURE)
\param mode The delivery mode of the message. (SYNC_MODE, ASYNC_MODE)
\param reqrepid A request-reply id which should match to that of a pending request.
\param pattern_id The pattern id corresponding to the message.
\param fptr A pointer to a function to destroy the message once it is processed
\return Returns zero on success, -1 on some error
\sa new_message(), new_notification(), new_request()
 */
int new_reply(Router *router, Message *m, int toolid, MessageStatus st, 
              MessageMode mode, int reqrepid, int pattern_id, MessageDestroy_fptr fptr);

//! Sends a message to the Router to deliver to the intended receivers.
/*! It inserts the message into the message queue. If the message is a synchronous message
    then send_sync_message() is invoked by this function.
\param router An instance of the Router 
\param message A pointer to the message
\return Returns zero on success, 1 on error
\sa send_sync_message()
 */
int send_message(Router *router, Message *message);

//! Sends a message to the Router to deliver to the intended receivers.
/*! It inserts the message into the message queue. When the message is a synchronous message
    then send_sync_message() does not return till the message has been delivered.
\param router An instance of the Router 
\param message A pointer to the message
\return Returns zero on success, 1 on error
\sa send_message()
 */
int send_sync_message(Router *router, Message *m);
  
//! The actual function which delivers the messages to the intended receivers.
/*! This is the function which processes all the messages in the queue and
    delivers them to the intended receivers based on the tool communication bindings
    computed by the Router.
\param router An instance of the Router 
\return Nothing
\sa dispatch_notification(), dispatch_reply(), dispatch_request()
 */
void dispatch(Router *router);

//! The function which delivers the notifications to the intended receivers.
/*! This function is invoked by function dispatch() if the message to be delivered is
    a notification. A notification is sent to all the interested listeners.
\param router An instance of the Router
\param message A pointer to the notification
\return Nothing
\sa dispatch(), dispatch_reply(), dispatch_request()
 */
void dispatch_notification (Router *router, Message *message);

//! The function which delivers the replies to the intended receivers.
/*! This function is invoked by function dispatch() if the message to be delivered is
    a reply. A reply is sent to all the interested listeners.
\param router An instance of the Router
\param message A pointer to the reply.
\return Nothing
\sa dispatch_notification(), dispatch(), dispatch_request()
 */
void dispatch_reply (Router *router, Message *message);

//! The function which delivers the requests to the intended receivers.
/*! This function is invoked by function dispatch() if the message to be delivered is
    a request. A request is sent to only one tool which succesfully processes the request.
\param router An instance of the Router
\param message A pointer to the request.
\return Nothing
\sa dispatch_notification(), dispatch(), dispatch_reply()
 */
void dispatch_request(Router *router, Message *message);

//! A \e helper function to copy an \e InputInterface over the other.
/*! This is a \e helper function which is not invoked directly. It is invoked by data structure
    libraries such as, generic list. This function copies a source input interface to the 
    destination input interface.
\param dest Destination input interface.
\param source Source input interface.
\return Nothing
*/

void duplicate_iface(InputInterface *dest,const InputInterface *source);

//! A \e helper function to destroy an \e InputInterface structure.
/*! This is a \e helper function which is not invoked directly. It is invoked by data structure
    libraries such as, generic list. This function is empty. 
\param i An input interface to destroy
\return Nothing
 */
void destroy_iface(void *i);

//! A \e helper function to destroy a \e RequestTool structure.
/*! This is a \e helper function which is not invoked directly. It is invoked by data structure
    libraries such as, generic list. This function is empty.
\param rt A pointer to the RequestTool structure.
\return Nothing
 */
void del_request_tool_func(RequestTool *rt);

//! A \e helper function to compare two message patterns.
/*! This is a \e helper function which is not invoked directly. It is invoked by data structure
    libraries such as, generic list. This function compares two message pattern strings.
\param mp1 A pointer to the first message pattern.
\param mp2 A pointer to the second message pattern.
\return 1 if two message patterns match, 0 otherwise.
 */
int match_pattern(struct MessagePattern_tag *mp1, struct MessagePattern_tag *mp2);

//! A \e helper function to copy a RequestTool structure.
/*! This is a \e helper function which is not invoked directly. It is invoked by data structure
    libraries such as, generic list. This function copies source RequestTool structure to
    the destination RequestTool structure.
\param dest The destination RequestTool structure.
\param source The source RequestTool structure.
\return 1 if a match is found, zero otherwise.
 */
void requesttool_copyfunc(RequestTool *dest, RequestTool *source);

//! A \e helper function to compare two RequestTool structures.
/*! This is a \e helper function which is not invoked directly. It is invoked by data structure
    libraries such as, generic list. This function compares two RequestTool structures.
\param rt1 A pointer to the first RequestTool structure.
\param rt2 A pointer to the second RequestTool structure.
\return 1 if a match is found, zero otherwise.
 */
int compare_reqrepid_func(RequestTool *rt1, RequestTool *rt2);

#endif

