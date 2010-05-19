#ifndef __MESSAGEQ_H
#define __MESSAGEQ_H

#include <pthread.h>
#include "generic_list.h"

#define MAX_DATA_ENTRIES 20

/**
    \brief Defines a message status to be either FAILURE or SUCCESS
*/
typedef enum { FAILURE=5, SUCCESS=10 } MessageStatus;

/**
    \brief Defines 3 different types of messages.
 */
typedef enum { REQUEST=10, REPLY=20, 
               NOTIFICATION=30 /*, FAILURE=40 */ } MessageType;

/**
    \brief Defines two types of message delivery modes.
*/
typedef enum { SYNC_MODE , ASYNC_MODE } MessageMode;
       
/**
    \brief Defines a type to indetify whether a synchonous message has been processes or not.
 */
typedef enum { PROCESSED, NOT_PROCESSED } SyncMessageStatus;
    
struct Message_tag; 

typedef void (*MessageDestroy_fptr)(struct Message_tag *);
// This function should not free the message pointer itself
// It should only free its dynamically allocated contents 
// if any
/**
    \brief Defines a message
 */

typedef struct Message_tag
{
    Glist_element __g; //!< For the use of generic list.
    int senderid;      //!< The sender of the message.
    int messageid;     //!< A unique message identifier
    MessageType type;  //!< Type of the message
    int reqrepid;      //!< A unique  request-reply identifier
    MessageStatus status;  //!< Status of the message.
    MessageMode mode;      //!< Mode of delivery of the message.
    SyncMessageStatus *sync_message_status; // checking this is valid
    // only if it is a SYNC_MODE message.
    int pattern_id; //!< The pattern id of the sender tool to which the message belongs.
    int data_entries;   //!< number of data entries
    MessageDestroy_fptr mdfptr;  //!< A pointer to a function to destroy the message after processing.
    void *data[MAX_DATA_ENTRIES]; //!< Storage for data.
} Message;

/**
    \brief Defines a message queue.
 */

typedef struct MessageQ_tag
{
    GLIST(Message) mlist;    //!< The message queue is implemented as a generic list of messages.
    pthread_mutexattr_t qmutex_attr; 
    pthread_mutex_t qmutex;  //!< A mutex to protect the queue from the concurrent access.
} MessageQ;

MessageQ * mqueue_init(MessageQ *mq);
void mqueue_deinit(MessageQ *mq);
int mqueue_empty(MessageQ *mq);
int mqueue_size(MessageQ *mq);
Message *mqueue_add(MessageQ *mq, Message *);
int mqueue_pophead(MessageQ *mq, Message *);
int mqueue_peephead(MessageQ *mq, Message *);
int mqueue_eraseany(MessageQ *mq, Message *);
Message *mqueue_search(MessageQ *mq, Message *to_search);

Message* reqrep_init(Message *m,int senderid,MessageStatus ms,MessageType mm,char *string);

void duplicate_message(Message *dest,const Message *source);
void clone_message(Message *dest,const Message *source);
void destroy_message(Message *m);
void mock_destroy_message(Message *m);
int compare_messages(const Message *to_search,const Message *inlist);
void print_message(Message *m);

#endif

