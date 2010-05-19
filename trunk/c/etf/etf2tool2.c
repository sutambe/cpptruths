#include <stdio.h>
#include <string.h>
#include "registrar.h"
#include "message_router.h"

static int patternid[MAX_PATTERNS];
static int myhandle;
static Router *rtr;

void del_message_func(Message *m);
void notification_receive(Message *m, char *pattern)
{
    Message message;
    int ret;
    
    if (strstr(pattern,"REDIRECT_LOOKUP"))
    {
        char *funcname=m->data[0];
        char *libname=m->data[1];
        
        if (0 == strcmp(funcname,"strrchr") && 
            0 == strcmp(libname,"main"))
        {
            ret=new_request(rtr, &message, myhandle, SUCCESS, SYNC_MODE,
                            patternid[0], del_message_func);
            message.data_entries=4;
            message.data[0]="strrchr";
            message.data[1]="main";
            message.data[2]="_jt_jumptable";
            message.data[3]="libstringtable.so";
            //ret=send_message(rtr, &message);
        }
    }
}

int request_receive (Message *m, char *pattern)
{
    Message message;
    //printf("Tool2 request_receive: GOT %s\n",pattern);
    
    if(strstr(pattern,"MY_REQUEST"))
    {
        new_reply (rtr, &message, myhandle, SUCCESS, SYNC_MODE, 
                   m->reqrepid, patternid[1], del_message_func);
        send_message(rtr, &message);
    }    
    return 0; // SUCCESS 
}

int reply_receive (Message *m, char *pattern)
{
    printf("Tool2 reply_receive: GOT %s\n",pattern);
    return 0; // SUCCESS 
}

int tool_init(Registrar *r,int handle,void *master_dataptr)
{
    int ret;
    Message message;
    myhandle=handle;
    rtr=r->router_ptr;

    printf("Tool2 init: handle = %d, master_dataptr = %p\n",handle,master_dataptr);
    ret=register_input_pattern(r,handle,"REDIRECT_LOOKUP",NOTIFICATION);
    ret=register_input_pattern(r,handle,"MY_REQUEST",REQUEST);
    patternid[0]=register_output_pattern(r,handle,"REQUEST_SYM2TABLE",REQUEST);
    patternid[1]=register_output_pattern(r,handle,"MY_REPLY",REPLY);
    return TOOLINIT_SUCCESS;
}

void del_message_func(Message *m)
{
    // free contents of m only if they were allocated dynamically.
}
