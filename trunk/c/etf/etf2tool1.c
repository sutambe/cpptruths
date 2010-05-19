#include <stdio.h>
#include <string.h>
#include "registrar.h"
#include "message_router.h"

static int patternid;
static int myhandle;
static Router *rtr;

void del_message_func(Message *m);
extern int match_pattern(MessagePattern *mp1, MessagePattern *mp2);

int reply_receive (Message *m, char *pattern)
{
    if (strstr(pattern,"REPLY_SYM2SYM"))
    {
        //printf("Tool1 reply_receive: GOT %s\n",pattern);
    }
    return 0; // SUCCESS 
}

void notification_receive(Message *m, char *pattern)
{
    Message message;
    int ret;
    
    if (strstr(pattern,"REDIRECT_LOOKUP"))
    {
        char *funcname=m->data[0];
        char *libname=m->data[1];
        
        //printf("Tool1 notification_receive: GOT %s\n",pattern);
        
        if (0 == strcmp(funcname,"malloc") /* (&& 
            0 == strcmp(libname,"main") */ )
        {
            ret=new_request(rtr, &message, myhandle, SUCCESS, SYNC_MODE,
                            patternid, del_message_func);
            message.data_entries=4;
            message.data[0]="malloc";
            message.data[1]="*";
            message.data[2]="wrap_malloc";
            message.data[3]="libwrap.so.0.0";
            //ret=send_message(rtr, &message);
        }
    }
}

int tool_init(Registrar *r,int handle,void *master_dataptr)
{
    int ret;
    myhandle=handle;
    rtr=r->router_ptr;
    
    printf("Tool1 init: handle = %d, master_dataptr = %p, router = %p\n",handle,master_dataptr,rtr);
    ret=register_input_pattern(r,handle,"REDIRECT_LOOKUP",NOTIFICATION);
    register_input_pattern(r,handle,"REPLY_SYM2SYM",REPLY);
    patternid=register_output_pattern(r,handle,"REQUEST_SYM2SYM",REQUEST);
    return TOOLINIT_SUCCESS;
}

void del_message_func(Message *m)
{
    // free contents of m only if they were allocated dynamically.
}

