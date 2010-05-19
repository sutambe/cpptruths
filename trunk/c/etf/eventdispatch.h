/** \file eventdispatch.h
    \brief Event Dispatcher (ED) service and implementation interface.
*/

#ifndef __EVENTDISPATCH_H
#define __EVENTDISPATCH_H

#include "registrar.h"
#include "eventdispatch.h"
#include "messageq.h"
#include "message_router.h"
#include "common.h"

/**
    \brief An action notified by the modified linker(DDL) to the Event Dispatcher.

    This constant integer is internally used by the Event Dispatcher to know
    that the modifications to force the dynamic linker to load a symbol from 
    a particular library shall be invoked next.
 */

const int _EVENT_FORCE_LIBLOOKUP=-10;

/** \brief The types of actions notified by the modified dynamic linker(DDL) to the Event Dispatcher. 

    \e EVENT_LOOKUP signifies that DDL is going to invoke the _dl_redirect_lookup hook.
    \e EVENT_DEFINITION signifies that DDL is going to invoke the _dl_redirect_definition hook.
    \e EVENT_OFFSET signifies that DDL is going to invoke the _dl_redirect_offset hook.
    \e EVENT_SYMDEF signifies that DDL is going to invoke the _dl_redirect_symdef hook.
*/
typedef enum { EVENT_LOOKUP=10, EVENT_DEFINITION=20,
               EVENT_OFFSET=30, EVENT_SYMDEF=40 } EventType;

//! An interface which a slave tool must implement.
/*! Every slave tool must export a symbol named 'tool_init' to make use of
    DDL Framework and to work with the Event Dispatcher. This function is called
    once by the Event Dispatcher to let the tool initialise itself.
\param handle An opaque handle given by the Event Dispather(ED) to be used
       by the tool for any future invocation of the Event Dispatcher services.
\param master_dataptr Pointer to master tool's main data structure. It is upto
       the master tool to decide what to return as a master data structure. 
       the Event Dispather, in this case, is simply a carrier.
\param thread_id An integer which uniquely identifies the thread which invokes
       this function. This thread is not the \e dispatch_thread of the \e Router.
       For more information on the Router and the dispatch_thread see the ETF 
       framework overview.
\return Nothing.
\sa mastertool_init(), _fini() in the man pages of dlopen
*/
void tool_init(int handle , void *master_dataptr, int thread_id);

//! An interface which the master tool must implement.
/*! Master tool must export a symbol named 'mastertool_init' to make use of
    DDL Framework and to work with Event Dispatcher. This function is called
    once by Event Dispatcher to let master tool initialise itself.
\param reg A pointer to the instance of the Registrar of the ETF framework. 
       For more information on the Registrar see the ETF framework overview.
\param handle An opaque handle given by Event Dispather(ED) to be used
       by the master tool for any future invocation of the Event Dispatcher
       services. 
\param thread_id An integer which uniquely identifies the thread which invokes
       this function. This thread is not the \e dispatch_thread of the \e Router.
       For more information on the Router and the dispatch_thread see the ETF 
       framework overview.
\return Master tool is expected to return a pointer to its main data structure
        when it returns. This same pointer shall be passed to all the slave
        tools, if any. This gives a way of sharing same data structure
        among master and slave tools. Master and slave tools are responsible for
        maintaing the shared data structure consistent.
\sa tool_init(), _fini() in man pages of dlopen
*/
void *mastertool_init(Registrar *reg, int handle, int thread_id);

//! This function is used by the Event Dispatcher to send a failure reply to a tool.
/*! 
\param toolid The tool to which failure reply is to be sent.
\param reqrepid The failure reply should have the same request-reply id as that of the
       request. This is the paramter to pass that request-reply id.
\return Nothing.
*/
void request_failure_reply(int toolid, int reqrepid);

//! This is an API function used by the master tool to register its 'isactive' hook with the Event Dispatcher.
/*! 
\param fptr A pointer to a function which takes an \e EventType and \e thread_id as a parameter.
\return Nothing.
 */
void set_mastertool_isactive_callback(int (*fptr)(EventType event, int thread_id));

//! This function is used to 'destroy' the messages sent by the Event Dispatcher.
/*! The Router can't and does't interpret the contents of a messages. Therefore,
    it is upto the sender tool to destroy the contents of the sent message. This 
    means if there is some dynamically allocated memory for some data items in the 
    message, those must be freed by the sender tool. This function is invoked by 
    the Router when a message  has been processed by all the interested tools. This 
    is similar to the destructor of an object in C++.
\param message A pointer to the message to be destroied.
\return Nothing.
 */
void del_message_func(Message *message);

//! An interface implemented by the Event Dispatcher to receive notifications sent by other tools.
/*! The Event Dispatcher can receive notifications from other tools. For example, The Redirection
    Library. The Redirection Library sends \e DO_REDIRECTION and \e DO_OFFSET type of notifications
    to the Event Dispatcher to actually send that data back through the DDL hooks. 
    The Router invokes this function to notify the Event Dispatcher of such notifications.
\param message A pointer to the notification message.
\param pattern The pattern corresponding to the the message.
\return Nothing.
 */
void notification_receive(Message *message, char *pattern);

//! A function which registers the modified linker (DDL) as a tool of the Event Dispatcher.
/*! The job of this function is similar to the register_input_pattern function in registrar.h.
    The Event Dispatcher directly populates the internal data structure of the Registrar
    and makes its own entry in it. The Event Dispatcher can't directly use the API
    (register_input_pattern) provided by the Registrar because it uses \e dlopen() and \e dlsym()
    while doing that.
\param reg A pointer to the instance of the registrar.
\return Nothing.
 */
void register_linkertool(Registrar *reg);

//! The starting point of the \e dispatch_thread.
/*! The \e dispatch_thread begins its execution from this function. It never comes out
    of this function. This function invokes a function called dispatch in router.h to deliver
    the messages in the message queue. It is primarily used by the Router.
\param v A pointer to void. It is unused at this moment.
\return Nothing.
 */
void *dispatch_func(void *v);

//! Deinitialises the Event Dispatcher library.
/*! Frees all internal data structures.
\return Nothing.
\sa _dl_redirect_init()
 */
void _fini(void);

//! \b Callback function called by dynamic linker to initialise Event Dispatcher Library.
/*! This hook is inserted into dynamic linker code and it is called only once.
    This is to provide a chance to intialise Event Dispatcher.
\return Nothing.
\sa _dl_redirect_isactive(), _dl_redirect_lookup(), _dl_redirect_offset()
\sa _dl_redirect_definition(), _dl_redirect_symdef()
 */
void  _dl_redirect_init(int thread_id);

//! \b Callback function called by dynamic linker to check whether Event Dispatcher is 'active' or not.
/*! This hook is inserted into dynamic linker code and it is called everytime
    before invoking any other hook. If returned zero, Event Dispatcher gets a
    chance of turning off invocation of next immediate hook.
\param eventtype It represents next hook to be invoked by dynamic linker.
       Invocation of hooks is considered as events.
\return One if next hook callback is to be made, Zero otherwise.
\sa _dl_redirect_init(), _dl_redirect_lookup(), _dl_redirect_offset()
\sa _dl_redirect_definition(), _dl_redirect_symdef()
 */
int   _dl_redirect_isactive(EventType eventtype, int thread_id);

//! \b Callback function called by dynamic linker as a lookup hook.
/*! This hook is inserted into dynamic linker code and it is called
    when symbol lookup is performed.
\param funcname The function symbol dynamic linker is resolving right now.
\param filename The library (shared object) which is calling \e funcname function.
                and therefore, wants the symbol to be resolved.
\param lookup_libname An out parameter pointing to a library name to force the
                      dynamic linker to search the symbol.
\return Pointer to the symbol which actually gets resolved. If this symbol is different
        than the parameter \e funcname, we say redirection has been done.
\sa _dl_redirect_isactive(), _dl_redirect_init(), _dl_redirect_offset()
\sa _dl_redirect_definition(), _dl_redirect_symdef()
 */
char* _dl_redirect_lookup(char* funcname,char *filename, char **lookup_libname, int thread_id);

//! \b Callback function called by dynamic linker as a offset hook.
/*! This hook is inserted into dynamic linker code and it is called
    just before a GOT entry is updated in the calling shared object.
\param funcname The function symbol dynamic linker resolved right now.
\param libname The library (shared object) which is calling \e funcname function.
                and therefore, wants the symbol to be resolved.
\return An integer value of offset to be added in the symbol address (if at all)
        Zero otherwise.
\sa _dl_redirect_isactive(), _dl_redirect_lookup(), _dl_redirect_offset()
\sa _dl_redirect_definition(), _dl_redirect_symdef()
 */
int   _dl_redirect_offset(const char *funcname, const char *libname, int thread_id);

//! \b Callback function called by dynamic linker as a definition hook.
/*! This hook is inserted into dynamic linker code and it is called
    just after resolving the symbol returned by lookup hook.
\param new_funcname The symbol actually resolved by dynamic linker.
\param new_libname The library containing definition of symbol.
\param new_func_address Address of definition of resolved symbol.
\param orig_funcname The symbol dynamic linker started to resolve at the beginning.
                     \e lookup hook might change this symbol to
                     different one to achive redirection.
\param orig_libname The library (shared object) which is calling \e funcname function.
                and therefore, wants the symbol to be resolved.
\param orig_func_GOT_addr The address of GOT entry of \ orig_libname to be
       updated with the address of symbol resolved by dynamic linker.
\return Returns one if function finished successfully. Zero otherwise. Unused.
\sa _dl_redirect_isactive(), _dl_redirect_lookup(), _dl_redirect_offset()
\sa _dl_redirect_init(), _dl_redirect_symdef()
 */
int   _dl_redirect_definition(const char *new_funcname,
                              const char *new_libname,
                              const unsigned *new_func_address,
                              const char *orig_funcname,
                              const char *orig_libname,
                              const unsigned *orig_func_GOT_addr,
                              int thread_id);

//! \b Callback function called by dynamic linker as a symdef hook.
/*! This hook is inserted into dynamic linker code and it is called
    just after resolving the symbol in rtld.c source of dynamic linker.
\param symbol The symbol dynamic linker is resolving right now. (elf\\rtld.c)
\param libname Shared object in which definition of symbol was found.
\param address Address of symbol.\.
\param caller_libname Unused.
\return Returns 1 if no error occured. Unused.
\sa _dl_redirect_isactive(), _dl_redirect_lookup(), _dl_redirect_offset()
\sa _dl_redirect_init(), _dl_redirect_init()
 */
int _dl_redirect_symdef(const char *symbol, const char *libname,
                        unsigned *address, const char* caller_libname, int thread_id);


#endif // __EVENTDISPATCH_H


