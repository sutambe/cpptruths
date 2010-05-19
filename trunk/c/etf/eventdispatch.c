/** \file eventdispatch.c
    \brief Implements the Event Dispatcher (ED).
*/

#include <dlfcn.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "eventdispatch.h"
#include "messageq.h"
#include "message_router.h"
#include "registrar.h"
#include "common.h"

static char *const MAIN_STR="main";
static int (*mastertool_isactive)(EventType event, int thread_id)=0;
static int ed_redirect=0;
static int libc_start_main_done=0;
static int op_pattern_ids[MAX_PATTERNS]={0};
static int dispatch_thread_ready=0;
static char *newlink_name;
static int new_offset;
static Registrar registrar;
static Router router;
static pthread_mutexattr_t hooklock_attr;
static pthread_mutex_t hooklock;
pthread_t dispatch_thread;

void _fini(void)
{
    if (libc_start_main_done)
    {
        registrar_deinit(&registrar);
        pthread_mutex_destroy(&hooklock);
        pthread_mutexattr_destroy(&hooklock_attr);
    }
}
    
void  _dl_redirect_init(int thread_id)
{
    ed_redirect=0;
    // stuff here
    ed_redirect=1;
}
void eventdispatch_init(int thread_id)
{
   char *toolfile=0;
   int threadret=0;
   
   ed_redirect=0;
   
   pthread_mutexattr_init(&hooklock_attr);
   pthread_mutexattr_settype(&hooklock_attr,PTHREAD_MUTEX_RECURSIVE_NP);
   pthread_mutex_init(&hooklock, &hooklock_attr);
   
   registrar_init(&registrar);
   router_init(&router);
   
   router.registrar_ptr=&registrar;
   registrar.router_ptr=&router;
   
   register_linkertool(&registrar);

#ifdef DEBUG   
      printf("eventdispatch.c: Creating dispatch thread.\n");
#endif
      threadret=pthread_create(&dispatch_thread,NULL,dispatch_func,0);
   
      if(threadret)
         fprintf(stderr,"eventdispatch.c: Thread can't be created.\n");
#ifdef DEBUG
      else
         printf("eventdispatch.c: Thread created.\n");
#endif

   toolfile=getenv("LD_TOOLFILE") ?: LD_TOOLFILE;
   if(TOOLINIT_FAIL == load_tools(&registrar,toolfile))
   {
       fprintf(stderr,"Tool initialisation failed.\n");
       ed_redirect=0;
   }
   else
   {
        dump_patterns(&registrar,"PATTERNS");
        if(registrar.active_tools[MASTER_TOOL].isactive)
        {  // Master must be there.
            ed_redirect=1;
        }
        else
            ed_redirect=0;
      
      /*Tcl thread takes significant time to create new tcl interpreter.
            Therefore, we stop the application thread till Tcl thread
            is ready to process hook data. */
      while(!dispatch_thread_ready);
    }
#ifdef DEBUG
   printf("Returning from eventdispatch_init in eventdispatch.c.\n");
#endif
      
}

void *dispatch_func(void *v)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,0);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,0);
    
    dispatch_thread_ready=1;
    while(1)
    {
        dispatch(&router);
        pthread_testcancel();
        sched_yield();
    }
    return 0;
}

void register_linkertool(Registrar *r)
{
    MessagePattern *mp=0;
    ToolCommunications *linkertool=0;
    pthread_mutex_lock(&r->tool_mutex);
  
    linkertool=&r->active_tools[LINKER_TOOL];
    linkertool->toolid=LINKER_TOOL;
    linkertool->toollib="LINKER_TOOL";
    linkertool->lib_handle=0;
    linkertool->isactive=1;
    linkertool->ipcount=0;
    
    op_pattern_ids[0]=register_output_pattern(r,LINKER_TOOL,
                      "REDIRECT_LOOKUP" DDL_LINKFNAME DDL_LINKLIBNAME DDL_ADDRESS, NOTIFICATION);
    op_pattern_ids[1]=register_output_pattern(r,LINKER_TOOL,
                      "REDIRECT_OFFSET" DDL_LINKFNAME DDL_LINKLIBNAME DDL_INT, NOTIFICATION);
    op_pattern_ids[2]=register_output_pattern(r,LINKER_TOOL,
                      "REDIRECT_DEFINITION" DDL_DEFFNAME DDL_DEFLIBNAME DDL_ADDRESS 
                      DDL_LINKFNAME DDL_LINKLIBNAME DDL_ADDRESS, NOTIFICATION);
    op_pattern_ids[3]=register_output_pattern(r, LINKER_TOOL,
                      "REQUEST_FAILURE_NO_REPLY", REPLY);
    
    mp=&linkertool->input_pattern[linkertool->ipcount];
    mp->not_fptr=notification_receive;
    mp->pattern="DO_REDIRECTION";
    mp->type=NOTIFICATION;
    linkertool->ipcount++;
    Glist_add(&r->input_pattern_list, mp, sizeof(MessagePattern),(void *) copy_message_pattern);
    
    mp=&linkertool->input_pattern[linkertool->ipcount];
    mp->not_fptr=notification_receive;
    mp->pattern="DO_OFFSET";
    mp->type=NOTIFICATION;
    linkertool->ipcount++;
    Glist_add(&r->input_pattern_list, mp, sizeof(MessagePattern),(void *) copy_message_pattern);
    
    r->tool_count++;
    
    pthread_mutex_unlock(&r->tool_mutex);
}

void notification_receive(Message *m, char *pattern)
{
    if(strstr(pattern,"DO_REDIRECTION"))
    {
        newlink_name=m->data[0];
    }
    else if(strstr(pattern,"DO_OFFSET"))
    {
        new_offset=(int)m->data[0];
    }
}

int _dl_redirect_isactive(EventType e, int thread_id)
{
#ifdef DEBUG
   //printf("Inside _dl_redirect_isactive in eventdispatch.c\n");
#endif
      
    if(libc_start_main_done)
    {
       if(thread_id == dispatch_thread)
            return 0;

       return (ed_redirect && mastertool_isactive) ?
              (*mastertool_isactive)(e,thread_id) : 0;
    }
    else return ed_redirect; 
}

char* _dl_redirect_lookup(char* funcname,char *filename,char **lookup_libname, int thread_id)
{
    char *ret_name=funcname;
    Message message = { { 0 } };
    int temp_ed_redirect = ed_redirect;

    ed_redirect = 0;
#ifdef DEBUG
    printf("Inside _dl_redirect_lookup. funcname=%s\n",funcname);
#endif
    
    if(0==strcmp(funcname,"__libc_start_main"))
    {
        eventdispatch_init(thread_id);
        libc_start_main_done=1;
    }
    if(!libc_start_main_done)
    {
        ret_name=funcname;
    }
    else
    {
        pthread_mutex_lock(&hooklock); // unlock in _dl_redirect_offset
        if(-1 == new_notification(&router, &message, LINKER_TOOL, SUCCESS, SYNC_MODE,
                                  op_pattern_ids[0], del_message_func))
        {
            fprintf(stderr,"_dl_redirect_lookup: new_notification failed\n");
        }
        else
        {
            message.data_entries=4;
            message.data[0]=funcname;
            message.data[1]=*filename ? filename : MAIN_STR;
            message.data[2]=lookup_libname;
            message.data[3]=(void *)thread_id;
            newlink_name=0;
            send_message(&router, &message);
            ret_name=newlink_name;
        }
    }
    ed_redirect=temp_ed_redirect;
    return ret_name;
}
int _dl_redirect_offset(const char *funcname, const char *libname, int thread_id)
{
    int ret_value=0;
    int temp_ed_redirect = ed_redirect;
    Message message = { { 0 } };
    ed_redirect = 0;

#ifdef DEBUG
    printf("Inside _dl_redirect_offset. funcname=%s\n",funcname);
#endif

    if(libc_start_main_done)
    {
        new_notification(&router, &message, LINKER_TOOL, SUCCESS, SYNC_MODE,
                        op_pattern_ids[1], del_message_func);
        message.data_entries=3;
        message.data[0]=(char *)funcname;
        message.data[1]=(char *)(*libname ? libname : MAIN_STR);
        message.data[2]=(void *) thread_id;
        
        new_offset=0;
        send_message(&router, &message);
        ret_value=new_offset;
    }
    
    pthread_mutex_unlock(&hooklock); // lock in _dl_redirect_lookup
    ed_redirect=temp_ed_redirect;
    return ret_value;
}
int _dl_redirect_definition(const char *new_funcname,
                            const char *new_libname,
                            const unsigned *new_func_address,
                            const char *orig_funcname,
                            const char *orig_libname,
                            const unsigned *orig_func_GOT_addr,
                            int thread_id)
{
    int ret_value=0;
    int temp_ed_redirect = ed_redirect;
    Message message = { { 0 } };
    ed_redirect = 0;

#ifdef DEBUG
    printf("Inside _dl_redirect_definition. orig_funcname=%s\n",orig_funcname);
#endif

    if(libc_start_main_done)
    {
        if(-1 == new_notification(&router, &message, LINKER_TOOL, SUCCESS, SYNC_MODE,
                                  op_pattern_ids[2], del_message_func))
        {
            fprintf(stderr,"_dl_redirect_definition: new_notification failed\n");
        }
        else
        {
            message.data_entries=7;
            message.data[0]=(char *)new_funcname;
            message.data[1]=(char *)(*new_libname ? new_libname : MAIN_STR);
            message.data[2]=(void *)new_func_address;
            message.data[3]=(char *)orig_funcname;
            message.data[4]=(char *)(*orig_libname ? orig_libname : MAIN_STR);
            message.data[5]=(void *)orig_func_GOT_addr;
            message.data[6]=(void *) thread_id;
            send_message(&router, &message);
        }
    }
    ed_redirect=temp_ed_redirect;
    return ret_value;
}

int _dl_redirect_symdef(const char *symbol, const char *libname,
                        unsigned *address, const char* caller_libname, int thread_id)
{
    int ret_value = 0;
    int temp_ed_redirect = ed_redirect;
    ed_redirect = 0;

    if(!libc_start_main_done)
    {
        ed_redirect=temp_ed_redirect;        
        return ret_value;
    }
    ed_redirect=temp_ed_redirect;
    return ret_value;
}

void request_failure_reply(int toolid, int reqrepid)
{
    Message message = { { 0 } };

    new_reply(&router, &message, LINKER_TOOL, FAILURE, SYNC_MODE, reqrepid, 
              op_pattern_ids[3], del_message_func);

    send_message(&router, &message);
}

void set_mastertool_isactive_callback(int (*fptr)(EventType event, int thread_id))
{
    mastertool_isactive=fptr;
}

void del_message_func(Message *m)
{
    // free contents of m only if they were allocated dynamically.
}

