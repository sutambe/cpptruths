#include <assert.h>
#include <string.h>
#include <stdio.h>

#define __USE_UNIX98 /* for pthread_mutexattr_settype */
#include <pthread.h>

#include "generic_list.h"
#include "messageq.h"

extern unsigned int message_counter;
extern unsigned int request_reply_counter;

MessageQ *mqueue_init(MessageQ *mq)
{
    assert(mq);
    pthread_mutexattr_init(&mq->qmutex_attr);
    pthread_mutexattr_settype(&mq->qmutex_attr,PTHREAD_MUTEX_RECURSIVE_NP);
    pthread_mutex_init(&mq->qmutex,&mq->qmutex_attr);
    Glist_initialise(&mq->mlist);
    return mq;
}

void mqueue_deinit(MessageQ *mq)
{
    assert(mq);
    Glist_deinitialise(&mq->mlist,(GlistDestroyElementFuncType)mock_destroy_message);
    pthread_mutex_destroy(&mq->qmutex);
    pthread_mutexattr_destroy(&mq->qmutex_attr);
}

int mqueue_empty(MessageQ *mq)
{
    int ret=0;
    
    assert(mq);
    pthread_mutex_lock(&mq->qmutex);
    ret=Glist_empty(&mq->mlist);
    pthread_mutex_unlock(&mq->qmutex);    
    return ret; // YES(1)  No(0)
}

int mqueue_size(MessageQ *mq)
{
    int ret=0;
    
    assert(mq);
    pthread_mutex_lock(&mq->qmutex);
    ret=Glist_count(&mq->mlist);
    pthread_mutex_unlock(&mq->qmutex);    
    return ret; 
}

Message *mqueue_add(MessageQ *mq, Message *m)
{
    Message *ret=0;
    assert(mq && m);
    pthread_mutex_lock(&mq->qmutex);
    ret=Glist_add(&mq->mlist,m,sizeof(Message),(GlistCopyElementFuncType) duplicate_message);
    pthread_mutex_unlock(&mq->qmutex);
    return ret;
}

// Caller must clear the memory occupied by m->data[...] otherwise there is
// a memory leak.
int mqueue_pophead(MessageQ *mq, Message *m)
{
    int ret=0;
    assert(mq && m);
    pthread_mutex_lock(&mq->qmutex);
    if(Glist_empty(&mq->mlist))
        ret=1; // ERROR
    else
    {
        memcpy(m,mq->mlist.head,sizeof(Message));
        Glist_eraseptr(&mq->mlist,mq->mlist.head,
                      (GlistDestroyElementFuncType)mock_destroy_message);
        ret=0; // SUCCESS
    }
    pthread_mutex_unlock(&mq->qmutex);
    return ret;
}
int mqueue_peephead(MessageQ *mq, Message *m)
{
    int ret=0;
    assert(mq && m);
    pthread_mutex_lock(&mq->qmutex);
    if(Glist_empty(&mq->mlist))
        ret=1;
    else
    {
        memcpy(m,mq->mlist.head,sizeof(Message));
        ret=0;
    }
    pthread_mutex_unlock(&mq->qmutex);
    return ret;
}
/*
// messageq does not know how to delete any general message.
// It can remove one message from messageq but can't deallocate
// all the storage assigned to it, in general. You have to
// provide a message specific destruction handler.
int mqueue_eraseany(MessageQ *mq, Message *m)
{
    int ret;
    assert(mq && m);
    pthread_mutex_lock(&mq->qmutex);
    
    ret=Glist_erase(&mq->mlist,m,
                   (GlistCompareElementFuncType)compare_messages,
                   (GlistDestroyElementFuncType)mock_destroy_message);
    
    pthread_mutex_unlock(&mq->qmutex);
    return ret;
}
*/
Message *mqueue_search(MessageQ *mq, Message *to_search)
{
    Message *ret=0;
    assert(mq && to_search);
    pthread_mutex_lock(&mq->qmutex);

    ret=Glist_search(&mq->mlist,to_search,
                    (GlistCompareElementFuncType)compare_messages);
    pthread_mutex_unlock(&mq->qmutex);
    return ret;
}

void duplicate_message(Message *dest,const Message *source)
{
    *dest=*source;
}

void mock_destroy_message(Message *m)
{
}

int compare_messages(const Message *to_search,const Message *inlist)
{
    return 0;
}

void print_message(Message *m)
{
    printf("\
    senderid = %d \
    messageid = %d \
    type = %d \
    reqrepid = %d \
    status = %d \
    mode = %d \
    pattern_id = %d \
    mdfptr = %p \
    data_entries = %d\n",m->senderid,m->messageid,m->type,m->reqrepid,
    m->status,m->mode,m->pattern_id,m->mdfptr,m->data_entries);

}

