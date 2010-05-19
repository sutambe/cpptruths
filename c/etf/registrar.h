#ifndef __REGISTRAR_H
#define __REGISTRAR_H

#include <stdio.h>
#include "common.h"
#include "message_router.h"

#ifndef LINKER_TOOL 
#define LINKER_TOOL 0
#endif

#ifndef MASTER_TOOL
#define MASTER_TOOL 1
#endif

#define TOOLINIT_SUCCESS 0
#define TOOLINIT_FAIL 1

#define MASTERTOOL_INIT "mastertool_init"
#define TOOL_INIT "tool_init"
#define LD_TOOLFILE "tools.input"

#define DDL_LINKFNAME " DDL_LINKFNAME "
#define DDL_LINKLIBNAME " DDL_LINKLIBNAME "
#define DDL_DEFFNAME " DDL_DEFFNAME " 
#define DDL_DEFLIBNAME " DDL_DEFLIBNAME "
#define DDL_INT " DDL_INT "
#define DDL_CHAR " DDL_CHAR "
#define DDL_STRING " DDL_STRING "
#define DDL_ADDRESS " DDL_ADDRESS "
#define DDL_DOUBLE " DDL_DOUBLE "
#define DDL_MOREDATA " DDL_MOREDATA "

#define NOTIFICATION_RECEIVE "notification_receive"
#define REQUEST_RECEIVE "request_receive"
#define REPLY_RECEIVE "reply_receive"

/**
    \brief Defines a type of pointer to a callback funtion to receive notifications.
 */
typedef void (*Notification_fptr)(Message *, char *);

/**
    \brief Defines a type of pointer to a callback funtion to receive requests.
 */
typedef int (*Request_fptr)(Message *, char *);
           
/**
    \brief Defines a type of pointer to a callback funtion to receive replies.
*/
typedef int (*Reply_fptr)(Message *, char *);

/**
    \brief A structure which implements a message pattern.
 */
typedef struct MessagePattern_tag
{
    Glist_element __g; //!< For the use of the generic list.
    char *pattern;     //!< A pointer to the string representation of the pattern
    MessageType type;  //!< Type of the pattern (Notification/Request/Reply)
    Notification_fptr not_fptr; //!< A pointer to a function which receives a notification
    Request_fptr req_fptr;      //!< A pointer to a function which receives a request
    Reply_fptr rep_fptr;        //!< A pointer to a function which receives a reply
} MessagePattern;

/**
    \brief A structure which holds information about a tool.
                   
    The Registrar maintains a \e ToolCommunications structure for every tool. ToolCommunications
    structure centralizes all the information about a tool such as, toolid, its library, 
    its input and output patterns etc. The Router uses this structure to compute the 
    tool communication bindings.
 */
typedef struct ToolCommunications_tag
{
    int toolid;        //!< Represents a unique idento\ifier for a tool.
    char *toollib;     //!< The library (.so file) in which tool has been implemented
    void *lib_handle;  //!< A handle to the library. for the use of dlopen(), dlsym() interface
    int isactive;      //!< Set to 1 if the tool is active, otherwise zero.
    MessagePattern input_pattern[MAX_PATTERNS]; //!< An array of all the input patterns of the tool.
    int ipcount; //!< Number of input patterns 
    int failrep_pattern; //!< Set to 1 it failure reply pattern has been registered, zero otherwise.
    MessagePattern output_pattern[MAX_PATTERNS]; //!< An array of all the output patterns of the tool.
    int opcount; //!< Number of output patterns
} ToolCommunications;

/**
    \brief Represents an instance of a registrar.
                   
    The Registrar maintains a \e ToolCommunications structure of all the tools. The Registrar manages
    registration of input and output patterns by tools. It also manages initilization of tools and
    assigning them unique tool identifiers.
 */
typedef struct Registrar_tag
{
    ToolCommunications active_tools[MAX_TOOLS]; //!< ToolCommunication information of all the tools
    pthread_mutexattr_t tool_mutex_attr; //!< tool_mutex attributes
    pthread_mutex_t tool_mutex;          //!< A mutex to serialize the access to all the fields of the Registrar
    int tool_count; //!< Number of tools loaded
    char *toolfile_envstr;  //!< Pointer to the string which shows where the tool file is loacated
    void *master_dataptr; //!< A pointer to the data structure returned by the master tool.
    struct Router_tag *router_ptr; //!< A pointer to the Router
    GLIST(MessagePattern) input_pattern_list; //!< A list of all the registered input patterns.
    GLIST(MessagePattern) output_pattern_list; //!< A list of all the registered output patterns.
} Registrar;

/**
    \brief Represents an iterator over the pattern lists of the Registrar.

    It represents an iterator over either the list of input patterns or the list of the output pattens.
 */
typedef struct PatternListIterator_tag
{
    GLIST_ITERATOR(MessagePattern) itr; //!< An iterator over a generic list.
    Registrar *reg; //!< Represents an instance of the Registrar.
} PatternListIterator;

//! Initializes a registrar.
/*! Initializes a registrar.
\param reg An instance of the Registrar to initialize.
\return Nothing.
\sa registrar_deinit()
 */
void registrar_init(Registrar *reg);

//! Deinitializes a registrar.
/*! Deinitializes a registrar.
\param reg An instance of the Registrar to de-initialize.
\return Nothing.
\sa registrar_init()
 */
void registrar_deinit(Registrar *reg);

//! Deinitializes a tool.
/*! Deinitializes a tool.
\param reg An instance of the Registrar from which the entry of the tool must be removed.
\param tool A pointer to the tool which is to be de-initialzed
\return Nothing.
\sa load_tools()
 */
void tool_deinit(Registrar *reg, ToolCommunications *tool);

//! Initializes the master tool.
/*! Initializes the master tool which is a special tool in ETF.
\param reg An instance of the Registrar.
\param tool A pointer to an entry in the array of tool communications structure to populate
\param toollib A pointer to the filename (with the path) of the master tool library.
\return A pointer to the main data structure of the master tool which can used by slave tools.
\sa load_tools(), initilise_slavetool()
 */
void *initialise_mastertool(Registrar *reg, ToolCommunications *tool, const char *toollib);

//! Initializes a slave tool.
/*! Initializes a slave tool.
\param reg An instance of the Registrar
\param tool A pointer to an entry in the array of tool communications structure to populate
\param toollib A pointer to the filename (with the path) of the slave tool library.
\return A pointer to the main data structure of the master tool which can used by slave tools.
\sa load_tools(), initilise_mastertool()
 */
int initialise_slavetool(Registrar *reg, ToolCommunications *tool, const char *toollib);

//! Loads tools from a file.
/*! Loads tools from a file.
\param reg An instance of the Registrar
\param toolfile A string representing the name of the file in which tool shared object files are listed.
\return zero on success one otherwise
\sa initilise_mastertool(), initialise_slavetool()
 */
int load_tools(Registrar *reg, const char *toolfile);

//! Registers an input pattern with the Registrar.
/*! To receive messages generated by an informer tool a listener tool should register an input
    pattern of exactly same name. The input pattern need not have the data types, just the pattern
    name is sufficient. Depending upon the type of the pattern, the Registrar tries to locate 
    the corresponding "receive" function. For example, for request type, "request_receive" function
    must have been exported by the tool shared object.
\param reg An instance of the Registrar
\param toolid The identifier of the tool (handle provided by the Event Dispatcher)
\param pattern A pointer to the string representing the name of the pattern
\param type Type of the messages generated by the informer tool (NOTIFICAITON/REQUEST/NOTIFICATION)
\return The pattern identifier on success, -1 otherwise
\sa register_output_pattern()
 */
int register_input_pattern(Registrar *reg, int toolid, char *pattern, MessageType type);

//! Registers an output pattern with the Registrar.
/*! To generate messages, every tool must register at least one output pattern. The output pattern string
    should contain the pattern name as well as the DDL data types, if any.
\param reg An instance of the Registrar
\param toolid The identifier of the tool (handle provided by the Event Dispatcher)
\param pattern A pointer to the string representing the name and data types in the pattern 
\param type Type of the messages generated by the pattern (informer tool) (NOTIFICAITON/REQUEST/NOTIFICATION)
\return The pattern identifier on success, -1 otherwise
\sa register_input_pattern()
 */
int register_output_pattern(Registrar *reg, int toolid, char *pattern, MessageType type);

//! Gets the instance of the Router
/*! Gets the instance of the Router
\param reg An instance of the Registrar
\return An instance of the Router.
 */
struct Router_tag *get_router(const Registrar *reg);

//! This function initiates an iterator either on input pattern list or the output pattern list or the Registrar.
/*! The function which initiates an iterator either on input pattern list or the Registrar 
    or the output pattern list. 
\param reg An instance of the Registrar
\param which 0 means input pattern list, 1 means output pattern list.
\return A pattern list iterator depending upon the value of \e which.
\sa pattern_itr_at(), pattern_itr_next()
 */
PatternListIterator pattern_itr_begin(Registrar *reg, int which);

//! Returns the mesage pattern pointed to by the pattern list iterator.
/*! This function populates the message pattern pointed by \e mp with the
    message pattern pointed by the pattern list iterator \e itr and the
    function returns 1. If itr is pointing at the end of the list, the 
    function returns zero and the message pattern pointed by mp remains untouched.
    The same function works on input pattern list as well as the output pattern list. 
\param itr An instance of a pattern list iterator (input or output)
\param mp A message pattern passed by reference which gets populated upon successful execution of the function.
\return 1 if a message pattern was found at the iterator's position, zero otherwise
\sa pattern_itr_next(), pattern_itr_begin()
 */
int pattern_itr_at(PatternListIterator itr, MessagePattern *mp);

//! Moves the pattern list iterator to the next pattern in the list.
/*! This function advances the pattern list iterator (input or output) to the next
    message pattern in the list. The same function works on input pattern list as well 
    as the output pattern list. 
\param itr An instance of a pattern list iterator (input or output)
\return A pattern list iterator advanced by one position.
\sa pattern_itr_begin(), pattern_itr_at()
 */
PatternListIterator pattern_itr_next(PatternListIterator itr);

//! Dumps all the registered input and output patterns in the specified file
/*! Dumps all the registered input and output patterns in the specified file
\param reg An instance of the registrar.
\param filename An output file name. 
\return nothing.
 */
void dump_patterns(Registrar *reg, char *filename);

//! Reads tool file, dlopens tool shared objects and initialises them.
/*! This function reads tool shared objects from tool file. Tool shares
    objects are written one below the another.
\param hookclients A Pointer to \e HookEventListener array of Event Dispatcher.
\param toolfile_name Name of tool file.
\return Nothing
*/
int toolfile_read(Registrar *r,const char *toolfile_name);

//! Skips a line in given input file.
/*! Skips everything upto next newline character in file. File handle shoule be
    open and is not closed by the function even after EOF.
\param infile An open file handle. File handle is not closed.
\return EOF, if EOF is encountered. '\n' otherwise (if present)
*/
int skipline(FILE *infile);

//! A \e helper function to copy two message patterns.
/*! This is a \e helper function which is not invoked directly. It is invoked by data structure
    libraries such as, generic list. This function makes a copy of souce message pattern and
    stores it in the destination message pattern.
\param dest A pointer to the destination message pattern.
\param source A pointer to the source message pattern.
\return Nothing
 */
void copy_message_pattern(MessagePattern *dest, MessagePattern *source);

//! A \e helper function to destroy a message patterns.
/*! This is a \e helper function which is not invoked directly. It is invoked by data structure
    libraries such as, generic list. This function is empty.
\param mp A pointer to a message pattern
\return Nothing
 */
void destroy_message_pattern(MessagePattern *mp);

#endif

