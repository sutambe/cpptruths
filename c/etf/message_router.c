#include <assert.h>

#include "common.h"
#include "message_router.h"
#include "registrar.h"
#include <string.h>
#define __USE_UNIX98 /* for pthread_mutexattr_settype */
#include <pthread.h>
#include <values.h>

extern pthread_t dispatch_thread; // eventdispatch.c 
extern void request_failure_reply(int toolid, int reqrepid); // eventdispatch.c 

void router_init(Router *r)
{
    int i=0, j=0;
    
    for(i=0;i < MAX_TOOLS;i++)
    {
        for(j=0;j < MAX_PATTERNS;j++)
            Glist_initialise(&r->comm_bindings[i].pattern_bindings[j]);
        
        r->comm_bindings[i].pattern_count=0;
    }
    r->registrar_ptr=0;
    r->message_counter=0;
    r->request_reply_counter=0;
    Glist_initialise(&r->request_tool_list);
    mqueue_init(&r->mqueue);
    pthread_mutexattr_init(&r->bindings_mutex_attr);
    pthread_mutexattr_settype(&r->bindings_mutex_attr,PTHREAD_MUTEX_RECURSIVE_NP);
    pthread_mutex_init(&r->bindings_mutex, &r->bindings_mutex_attr);
}

int compute_commbindings(Router *router, Registrar *registrar)
{
    int i=0, j=0 ,a=0, b=0;
    MessagePattern *op=0, *ip=0;
    InputInterface iface = { { 0 } };    
    
    assert(router && registrar);
    pthread_mutex_lock(&router->bindings_mutex);
    for(i=0;i < registrar->tool_count;i++)
    {
        /* This part of the code(inner loop of j) determines the order in which 
           messages are sent to tools. Tool zero receives it the last. Tool 
           zero is LINKER_TOOL. Before that is MASTER_TOOL. We want 
           MASTER_TOOL to receive the REDIRECT_LOOKUP, REDIRECT_OFFSET, 
           REDIRECT_DEFINITION at last. i.e. after all tools. This way 
           of implicit ordering is not good. Actually one should have here 
           so called "message delivery constraints" */
        for(j=registrar->tool_count-1; j >=0; j--)
        {
            if(i == j) 
                continue;
            
            for(a=0;a < registrar->active_tools[i].opcount;a++)
            {
                for(b=0;b < registrar->active_tools[j].ipcount;b++)
                {
                    op=&registrar->active_tools[i].output_pattern[a];
                    ip=&registrar->active_tools[j].input_pattern[b];
                    if(match_pattern(op,ip))
                    {
                        iface.toolid=j;
                        iface.pattern_id=b;
                        Glist_add(&router->comm_bindings[i].pattern_bindings[a],
                                  &iface,sizeof(InputInterface),
                                  (GlistCopyElementFuncType)duplicate_iface);
                        //printf("tool%d:%d to tool%d:%d\n",i,a,j,b);
                    }
                }
            }
        }
    }
    pthread_mutex_unlock(&router->bindings_mutex);
    return 0;
}

void flush_commbindings(Router *router)
{
    int i=0, j=0;
    
    pthread_mutex_lock(&router->bindings_mutex);
    for(i=0;i < MAX_TOOLS;i++)
    {
        for(j=0;j < MAX_PATTERNS;j++)
            Glist_deinitialise(&router->comm_bindings[i].pattern_bindings[j],
                               (GlistDestroyElementFuncType)destroy_iface);
        
        router->comm_bindings[i].pattern_count=0;
    }
    pthread_mutex_unlock(&router->bindings_mutex);
}

void router_deinit(Router *router)
{
    pthread_mutex_destroy(&router->bindings_mutex);
    pthread_mutexattr_destroy(&router->bindings_mutex_attr);
    flush_commbindings(router);
    mqueue_deinit(&router->mqueue);
}

int validate_para(Router *router, int toolid, int pattern_id)
{
    Registrar *reg=0;
    ToolCommunications *tool=0;
    int ret=0;
    
    reg=router->registrar_ptr;
    pthread_mutex_lock(&reg->tool_mutex);

    if(toolid >= reg->tool_count)
        ret=-1; // ERROR
    else if ((tool=&reg->active_tools[toolid]) && 
             (pattern_id >= tool->opcount || !tool->output_pattern[pattern_id].pattern))
        ret=-1; // ERROR 
    pthread_mutex_unlock(&reg->tool_mutex);
    return ret;
}

int new_message(Router *router, Message *m, int toolid, MessageStatus st, 
                MessageMode mode, int pattern_id, MessageDestroy_fptr fptr)
{
    int ret=0;
    
    assert(router && m && fptr);
    ret=validate_para(router, toolid, pattern_id);    
    
    if(-1 != ret)
    {
        m->senderid=toolid;
        pthread_mutex_lock(&router->bindings_mutex);
        m->messageid=router->message_counter++;
        pthread_mutex_unlock(&router->bindings_mutex);
        m->status=st;
        m->mode=mode;
        m->type=-1; // set in more specific function
        m->reqrepid=-1; // set in more specific function
        m->sync_message_status=0; // action taken in function send_message
        m->pattern_id=pattern_id;
        m->data_entries=0;
        m->mdfptr=fptr;
        ret=0; // SUCCESS
    }
    return ret;
}

int new_notification(Router *router, Message *m, int toolid, MessageStatus st, 
                     MessageMode mode, int pattern_id, MessageDestroy_fptr fptr)
{
    int ret=0;
    
    ret=new_message(router,m,toolid,st,mode,pattern_id,fptr);
    if(-1 != ret)
    {
        m->type=NOTIFICATION;
        m->reqrepid=-1;
        ret=0; // SUCCESS
    }
    return ret;
}

int new_request(Router *router, Message *m, int toolid, MessageStatus st, 
                MessageMode mode, int pattern_id, MessageDestroy_fptr fptr)
{
    int ret=0;
    
    ret=new_message(router,m,toolid,st,mode,pattern_id,fptr);
    if(-1 != ret)
    {
        m->type=REQUEST;
        pthread_mutex_lock(&router->bindings_mutex);
        m->reqrepid=router->request_reply_counter++;
        pthread_mutex_unlock(&router->bindings_mutex);
        ret=0; // SUCCESS
    }
    return ret;
}

int new_reply(Router *router, Message *m, int toolid, MessageStatus st, 
              MessageMode mode, int reqrepid, int pattern_id, MessageDestroy_fptr fptr)
{
    int ret=0;
   
    ret=new_message(router,m,toolid,st,mode,pattern_id,fptr);
    
    if(-1 != ret)
    {
        RequestTool rt;        
        pthread_mutex_lock(&router->bindings_mutex);
        
        rt.reqrepid=reqrepid;
        if (!Glist_search(&router->request_tool_list,&rt,(void *)compare_reqrepid_func))
            ret=-1; // ERROR No request pending for this reply.
        else
        {
            m->type=REPLY;
            m->reqrepid=reqrepid;
            ret=0; // SUCCESS
        }
        pthread_mutex_unlock(&router->bindings_mutex);
    }
    return ret;
}

int send_message(Router *router, Message *m)
{
    int ret=0;
    assert(router && m);

    if(REQUEST == m->type)
    {
        GLIST(InputInterface) * iface_list;
        pthread_mutex_lock(&router->bindings_mutex);    
        iface_list=&(router->comm_bindings[m->senderid].pattern_bindings[m->pattern_id]);
        if (Glist_empty(iface_list))
        {
            /* There is no tool to honor this request so I do not accept this reqeust */
            ret=1; /* ERROR */
        }
        else
        {
            RequestTool rt;
            rt.toolid=m->senderid;
            rt.reqrepid=m->reqrepid;
            Glist_add(&router->request_tool_list, &rt, sizeof(RequestTool),
                      (void *)requesttool_copyfunc);
            ret=0;
        }        
        pthread_mutex_unlock(&router->bindings_mutex);
    }
    
    if(0 == ret)
    {    
        if (SYNC_MODE == m->mode)
            ret=send_sync_message(router, m);
        else
            ret=mqueue_add(&router->mqueue, m)? 0 /* SUCCESS */ : 1 /* ERROR */;
    }
    return ret;
}

int send_sync_message(Router *router, Message *m)
{
    int ret=0;
    /* sync_message_status is a pointer to a location allocated on heap
        because, we have separate identity of its 'status' per sync message.
        This is necessary because, internal data structures (such as messageq)
        makes copies of messages. Therefore, we don't loose the identity of 
        status because pointer is also copied. 
    */
    m->sync_message_status=malloc(sizeof(SyncMessageStatus));
    assert(m->sync_message_status);
    *m->sync_message_status=NOT_PROCESSED;
    
    ret=mqueue_add(&router->mqueue, m)? 0 /* SUCCESS */ : 1 /* ERROR */;
    
    if(0 == ret)
    {
        if (pthread_self() == dispatch_thread)
            dispatch(router);  
        else
        {
            // Wait till SYNC message m is yet to be processed by dispatch thread
            while (NOT_PROCESSED == *m->sync_message_status)
                sched_yield();
        }    
        free(m->sync_message_status); // SYNC message m processed 
    }
    return ret;
}

void dispatch(Router *r)
{
    Message m = { { 0 } };
    const static int CIRCULARITY_THRESHOLD=50;
    static int circularity_count=0;

    if(CIRCULARITY_THRESHOLD == ++circularity_count)
    {
        fprintf(stderr,"ERROR: CIRCULARITY_THRESHOLD reached. Message neglected.\n");
        mqueue_pophead(&r->mqueue,&m);
        circularity_count--;
        return;
    }
    while (!mqueue_empty(&r->mqueue))
    {
        if (1 == mqueue_pophead(&r->mqueue,&m))
        {
            fprintf(stderr,"ERROR: message_router.c:dispatch\n");
            return;
        }
        switch (m.type)
        {
            case NOTIFICATION:
                dispatch_notification(r, &m);
                break;
            case REQUEST:
                dispatch_request(r, &m);
                break;
            case REPLY:
                dispatch_reply(r, &m);
                break;
        }
        if (SYNC_MODE == m.mode)
            *m.sync_message_status=PROCESSED;
        (*m.mdfptr)(&m); // destroy the CONTENTS of the message.
    }
    circularity_count--;
}

void dispatch_notification (Router *r, Message *m)
{
    Registrar *reg=0;
    InputInterface *iface=0;
    MessagePattern *mp=0;
    GLIST(InputInterface) *tool_list=0;
    GLIST_ITERATOR(InputInterface) iface_itr=0;

    reg=r->registrar_ptr;
    pthread_mutex_lock(&r->bindings_mutex);
    
    tool_list=&(r->comm_bindings[m->senderid].pattern_bindings[m->pattern_id]);
    iface_itr=Glist_itr_begin(tool_list);
    while (iface_itr)
    {
        iface=Glist_itr_at(iface_itr);
        mp=&(reg->active_tools[iface->toolid].input_pattern[iface->pattern_id]);
        (*mp->not_fptr)(m, mp->pattern);
        iface_itr=Glist_itr_next(iface_itr);
    }
    pthread_mutex_unlock(&r->bindings_mutex);
}

void dispatch_reply (Router *r, Message *m)
{
    RequestTool rt = { { 0 } }, *rtptr=0;
    Registrar *reg = 0;
    InputInterface *iface=0;
    MessagePattern *mp=0;
    GLIST(InputInterface) *tool_list=0;
    GLIST_ITERATOR(InputInterface) iface_itr=0;
    
    reg=r->registrar_ptr;
    pthread_mutex_lock(&r->bindings_mutex);
    
    rt.reqrepid=m->reqrepid;
    rtptr=Glist_search(&r->request_tool_list,&rt,(void *)compare_reqrepid_func);
    if (!rtptr)
    {
        fprintf(stderr,"message_router.c: dispatch: A REPLY for no corresopnding REQUEST\n");
        /* TODO take proper error handling steps */
    }
    else
    {
        tool_list=&(r->comm_bindings[m->senderid].pattern_bindings[m->pattern_id]);
        iface_itr=Glist_itr_begin(tool_list);
        
        while (iface_itr)
        {
            iface=Glist_itr_at(iface_itr);
            if (rtptr->toolid == iface->toolid)
            {
                mp=&(reg->active_tools[iface->toolid].input_pattern[iface->pattern_id]);
                (*mp->rep_fptr)(m, mp->pattern);
                Glist_eraseptr(&r->request_tool_list,rtptr,(void *)del_request_tool_func);
                iface_itr=0;
            }
            iface_itr=Glist_itr_next(iface_itr);
        }
    }
    pthread_mutex_unlock(&r->bindings_mutex);
}

void dispatch_request(Router *r, Message *m)
{
    Registrar *reg;
    InputInterface *iface;
    MessagePattern *mp;
    GLIST(InputInterface) *tool_list;
    GLIST_ITERATOR(InputInterface) iface_itr;
    int request_processed=0;
    
    reg=r->registrar_ptr;
    pthread_mutex_lock(&r->bindings_mutex);
    
    tool_list=&(r->comm_bindings[m->senderid].pattern_bindings[m->pattern_id]);
    iface_itr=Glist_itr_begin(tool_list);
    
    while (iface_itr)
    {
        iface=Glist_itr_at(iface_itr);
        mp=&(reg->active_tools[iface->toolid].input_pattern[iface->pattern_id]);
        request_processed=(*mp->req_fptr)(m,mp->pattern)? 0 /*ERROR*/ : 1 /*SUCCESS*/;
        /* Only first successful reply is enough. */
        if(request_processed)
            break;
        iface_itr=Glist_itr_next(iface_itr);
    }
    pthread_mutex_unlock(&r->bindings_mutex);
    if (! request_processed)
    {
        /* request was not processed successfully, therefore, there won't
        be corresponding reply. Send pre-defined REQUEST_FAILURE_NO_REPLY 
        to sender of the reqeust.*/
        request_failure_reply(m->senderid, m->reqrepid);
    }
    else
    {
        /* A proper reply shall be sent therefore, keep the entry
        in the request_tool_list to be deleted later upon arrival of
        corresponding reply. */
    }
}

int match_pattern(MessagePattern *mp1, MessagePattern *mp2)
{
    char *space;
    space=strchr(mp1->pattern,' ');
    if (0==strncmp(mp1->pattern,mp2->pattern, space ? space - mp1->pattern : INT_MAX))
        return 1;
    else
        return 0;
}

void destroy_iface(void *i)
{
}

void duplicate_iface(InputInterface *dest,const InputInterface *source)
{
    *dest=*source;
}

void requesttool_copyfunc(RequestTool *dest, RequestTool *source)
{
    *dest=*source;
}

int compare_reqrepid_func(RequestTool *rt1, RequestTool *rt2)
{
    return rt1->reqrepid == rt2->reqrepid;
}

void del_request_tool_func(RequestTool *rt)
{

}

