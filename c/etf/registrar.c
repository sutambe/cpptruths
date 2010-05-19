#include <string.h>
#include <dlfcn.h>
#include <assert.h>

#include "registrar.h"
#include "message_router.h"

void registrar_init(Registrar *r)
{
    memset(r,0,sizeof(Registrar));
    Glist_initialise(&r->input_pattern_list);
    Glist_initialise(&r->output_pattern_list);
    pthread_mutexattr_init(&r->tool_mutex_attr);
    pthread_mutexattr_settype(&r->tool_mutex_attr,PTHREAD_MUTEX_RECURSIVE_NP);
    pthread_mutex_init(&r->tool_mutex, &r->tool_mutex_attr);
}

int register_input_pattern(Registrar *r, int toolid, char *pattern, MessageType type)
{
    ToolCommunications *tool=0;
    MessagePattern *mp=0;
    char *error=0;
    int pattern_id=0, ret=0;
    
    assert(r && pattern);
    pthread_mutex_lock(&r->tool_mutex);
    
#ifdef DEBUG
    fprintf(stderr,"registerrar.c: register_input_pattern, toolid=%d, pattern=%s\n",toolid,pattern);
#endif

    if((r->tool_count <= toolid) || (toolid != r->active_tools[toolid].toolid))
    {
        // toolid's don't match
        ret=-1; // ERROR
    }
    else if ((tool=&r->active_tools[toolid]) &&  (tool->ipcount > MAX_PATTERNS))
    {
        ret=-1; // ERROR
    }
    else
    {
        mp=&tool->input_pattern[tool->ipcount];
        pattern_id=tool->ipcount;
        switch(type)
        {
            case NOTIFICATION:
            {
                mp->not_fptr=dlsym(tool->lib_handle,NOTIFICATION_RECEIVE);
                if ((error = dlerror()) != NULL)
                {
                    fprintf (stderr, "ERROR: register_input_pattern: %s\n", error);
                    fprintf (stderr, "ERROR: NOTIFICATION, toolid=%d\n",toolid);
                    exit(EXIT_FAILURE);
                }
                break;
            }
            case REQUEST:
            {
                mp->req_fptr=dlsym(tool->lib_handle,REQUEST_RECEIVE);
                if ((error = dlerror()) != NULL)
                {
                    fprintf (stderr, "ERROR: register_input_pattern: %s\n", error);
                    fprintf (stderr, "ERROR: REQUEST, toolid=%d\n",toolid);
                    exit(EXIT_FAILURE);
                }
                break;
            }
            case REPLY:
            {
                mp->rep_fptr=dlsym(tool->lib_handle, REPLY_RECEIVE);
                if ((error = dlerror()) != NULL)
                {
                    fprintf (stderr, "ERROR: register_input_pattern: %s\n", error);
                    fprintf (stderr, "ERROR: REPLY, toolid=%d\n",toolid);
                    exit(EXIT_FAILURE);
                }
                break;
            }
        }
        mp->pattern=strdup(pattern);
        mp->type=type;
        tool->ipcount++;
        Glist_add(&r->input_pattern_list, mp, sizeof(MessagePattern),(void *) copy_message_pattern);
        flush_commbindings(r->router_ptr);
        compute_commbindings(r->router_ptr, r);
        ret=pattern_id;
    }
    pthread_mutex_unlock(&r->tool_mutex);

    return ret; // SUCCESS
}

int register_output_pattern(Registrar *r, int toolid, char *pattern, MessageType type)
{
    ToolCommunications *tool=0;
    MessagePattern *mp=0;
    int pattern_id=0, ret=0;
    
    assert(r && pattern);
    pthread_mutex_lock(&r->tool_mutex);

    if((r->tool_count < toolid) || (toolid != r->active_tools[toolid].toolid))
    {
        // toolid's don't match
        ret=-1; // ERROR
    }
    else if ((tool=&r->active_tools[toolid]) && (tool->opcount > MAX_PATTERNS))
        ret=-1; // ERROR
    else
    {
        mp=&tool->output_pattern[tool->opcount];
        pattern_id=tool->opcount;
        mp->not_fptr=0;
        mp->pattern=strdup(pattern);
        mp->type=type;
        tool->opcount++;
        Glist_add(&r->output_pattern_list, mp, sizeof(MessagePattern),(void *) copy_message_pattern);
        flush_commbindings(r->router_ptr);
        compute_commbindings(r->router_ptr, r);
        ret=pattern_id;
    }
    
    pthread_mutex_unlock(&r->tool_mutex);
    if (REQUEST == type && !tool->failrep_pattern)
    {
        register_input_pattern(r, toolid,"REQUEST_FAILURE_NO_REPLY", REPLY);
        tool->failrep_pattern=1;
    }
    return ret; // SUCCESS
}

int load_tools(Registrar *r, const char *toolfile)
{
    int retval=TOOLINIT_SUCCESS;
    pthread_mutex_lock(&r->tool_mutex);
    
    r->toolfile_envstr=strdup(toolfile);
#ifdef DEBUG
   printf("registrar.c: LD_TOOLFILE=%s\n",r->toolfile_envstr);
#endif

   if(TOOLINIT_FAIL == toolfile_read(r,r->toolfile_envstr))
   {
       fprintf(stderr,"Atleast one tool failed to load.\n");
       retval=TOOLINIT_FAIL;
   }
   
#ifdef DEBUG
   printf("registrar.c: Tool count = %d\n",r->tool_count);
#endif
    
    pthread_mutex_unlock(&r->tool_mutex);
    return retval;
}

void registrar_deinit(Registrar *r)
{
    int i=0;
    pthread_mutex_lock(&r->tool_mutex);
    for(i=r->tool_count-1;i >= MASTER_TOOL;--i)
    {
        tool_deinit(r,&r->active_tools[i]);
        r->tool_count--;
    }
    Glist_deinitialise(&r->input_pattern_list, (void *)destroy_message_pattern);
    Glist_deinitialise(&r->output_pattern_list, (void *)destroy_message_pattern);
    pthread_mutex_unlock(&r->tool_mutex);
    pthread_mutex_destroy(&r->tool_mutex);
    pthread_mutexattr_destroy(&r->tool_mutex_attr);
    memset(r->active_tools,0,sizeof(r->active_tools));
}

Router *get_router(const Registrar *reg)
{
    return reg->router_ptr;
}

void *initialise_mastertool(Registrar *r,ToolCommunications *tool,const char *toollib)
{
        char *initfunc=0, *error=0;
        void *master_dataptr=0;
        void *(*mtool_init)(Registrar *r, int handle, int thread_id)=0;
        
        /* It is important to note that RTLD_GLOBAL flag is
        required because tools depend on symbols exported
        by master library. Master shared object shall be
        searched for these symbols only if it is opened
        using RTLD_GLOBAL. For RTLD_NOW see below. */
#ifdef DEBUG
    printf("registrar.c: opening toollib: %s\n",toollib);
#endif
        tool->lib_handle=dlopen(toollib,RTLD_LAZY | RTLD_GLOBAL);
        if ((error = dlerror()) != NULL)
        {
            fprintf (stderr, "mastertool_init:dlopen: %s\n", error);
            return 0;
        }
        initfunc=MASTERTOOL_INIT;
        mtool_init=dlsym(tool->lib_handle,initfunc);
        if ((error = dlerror()) != NULL)
        {
            fprintf (stderr, "mastertool_init:dlsym: %s\n", error);
            return 0;
        }
        assert(mtool_init);
        tool->toollib=strdup(toollib);
        tool->failrep_pattern=0;
        tool->isactive=1;
        tool->toolid=r->tool_count++;
        master_dataptr=(*mtool_init)(r, r->tool_count, pthread_self());
        return master_dataptr;
}

int initialise_slavetool(Registrar *r, ToolCommunications *tool,const char *toollib)
{
        char *initfunc=0, *error=0;
        int (*tinit)(Registrar *r,int handle , void *master_dataptr)=0;
        /* It is important to note that RTLD_NOW flag is
            required because if tool calls eventdispather's methods
            in _fini of the tool, then the symbols won't be resolved.
            We solve the problem by resolving all the symbols at the
            beginning using RTLD_NOW. There is a problem here. ED's
            fini is called before tool's _fini. So tool can not call
            any ED API. This does not seem to be a right thing.
        */
#ifdef DEBUG
    printf("registrar.c: opening toollib: %s\n",toollib);
#endif

        tool->lib_handle=dlopen(toollib,RTLD_NOW);
        if ((error = dlerror()) != NULL)
        {
            fprintf (stderr, "%s\n", error);
            return TOOLINIT_FAIL;
        }
        initfunc=TOOL_INIT;
        tinit=dlsym(tool->lib_handle,initfunc);
        assert(tinit);
        tool->toollib=strdup(toollib);
        tool->toolid=r->tool_count++;
        tool->failrep_pattern=0;
        tool->isactive=1;
        if(TOOLINIT_FAIL == (*tinit)(r,tool->toolid,r->master_dataptr))
            return TOOLINIT_FAIL;
        else 
            return TOOLINIT_SUCCESS;
}

void tool_deinit(Registrar *r, ToolCommunications *tool)
{
        char *error=0;
        int i=0;
            
        pthread_mutex_lock(&r->tool_mutex);
        
        if(tool->isactive)
        {
            for(i=0;i < tool->ipcount;i++)
                free(tool->input_pattern[i].pattern);
            
            for(i=0;i < tool->opcount;i++)
                free(tool->output_pattern[i].pattern);
            
            if (dlclose(tool->lib_handle))
            {   
                error = dlerror();
                fprintf (stderr, "%s\n", error);
            }
    
            free(tool->toollib);
            tool->isactive=0;
        }
        pthread_mutex_unlock(&r->tool_mutex);

        /*
        if(tool->libhandle)
        {
            if (dlclose(tool->libhandle))
            {
                error = dlerror();
                fprintf (stderr, "%s\n", error);
            }
            There could be few links getting resolved beyond this point
            also. Especially when dlclose for master is called. It is
            best if tools "unhook" themselves when _fini is called.
            If tools's hooks are not supposed to be called during
            that process, we can set hclient->ed_isactive=0 to do it.
        }
        */
}

int toolfile_read(Registrar *r,const char *toolfile_name)
{
        int retval=TOOLINIT_SUCCESS;
        char toollib[MAX_NAME_LENGTH]={0};
        char error_msg[500]={0};
        FILE *infile=0;

        assert(r && toolfile_name);

        infile=fopen(toolfile_name,"r");
        if(!infile)
        {
            sprintf(error_msg,"ERROR (%s)",toolfile_name);
            perror(error_msg);
            retval=TOOLINIT_FAIL;
            return retval;
        }

        while(1)
        {
            if(EOF==fscanf(infile,"%s",toollib)) break;
            if('#'==*toollib)
            {
                if(EOF==skipline(infile)) break;
                continue;
            }
            if(MASTER_TOOL == r->tool_count) // Master Tool
            {
                r->master_dataptr=initialise_mastertool(r,&r->active_tools[MASTER_TOOL],toollib);
                if(!r->master_dataptr)
                {
                    fprintf(stderr,"registrar.c: Mastertool initialisation failed.\n");
                    retval=TOOLINIT_FAIL;
                    break;
                }
                if(EOF==skipline(infile)) break;
            }
            else
            {
                if(TOOLINIT_FAIL == initialise_slavetool(r,&r->active_tools[r->tool_count],toollib))
                {
                    r->active_tools[r->tool_count].isactive=0;
                    retval=TOOLINIT_FAIL;
                }
                if(EOF==skipline(infile)) break;
            }
        }
        fclose(infile);
        return retval;
}

int skipline(FILE *infile)
{
     while(1)
     {
           int c=fgetc(infile);
           if(EOF==c) return EOF;
           else if('\n'==c) return '\n';
     }
}

void copy_message_pattern(MessagePattern *dest, MessagePattern *source)
{
    *dest=*source;
}

void destroy_message_pattern(MessagePattern *mp)
{

}

PatternListIterator pattern_itr_begin(Registrar *r, int which)
{
    PatternListIterator ret = { 0 };
    switch(which)
    {
        case 0: /* INPUT */
            ret.itr=Glist_itr_begin(&r->input_pattern_list);
            break;
        case 1: /* OUTPUT */
            ret.itr=Glist_itr_begin(&r->output_pattern_list);
            break;
    }
    ret.reg=r;
    return ret;
}

int pattern_itr_at(PatternListIterator itr, MessagePattern *mp)
{
    if (0 == itr.itr)
        return 0;
        
    pthread_mutex_lock(&itr.reg->tool_mutex);
    *mp=*((MessagePattern *)Glist_itr_at(itr.itr));
    pthread_mutex_unlock(&itr.reg->tool_mutex);
    
    return 1;
}

PatternListIterator pattern_itr_next(PatternListIterator itr)
{
    PatternListIterator ret = { 0 };
    ret=itr;
    pthread_mutex_lock(&itr.reg->tool_mutex);
    ret.itr=Glist_itr_next(itr.itr);
    pthread_mutex_unlock(&itr.reg->tool_mutex);
    return ret;
}

void dump_patterns(Registrar *r, char *filename)
{
    PatternListIterator iterator;
    MessagePattern mp;
    FILE *outfile=0;
    char error_msg[500]={0};    
    
    outfile=fopen(filename,"w");
    if(!outfile)
    {
        sprintf(error_msg,"ERROR (%s)",filename);
        perror(error_msg);
        return;
    }
    fprintf(outfile,"__INPUT PATTERNS:\n");
    iterator = pattern_itr_begin(r, 0 /* INPUT */ );
    while (pattern_itr_at(iterator, &mp))
    {
        fprintf(outfile,"%d %s\n",mp.type,mp.pattern);
        iterator = pattern_itr_next(iterator);
    }
    fprintf(outfile,"\n");
    fprintf(outfile,"__OUTPUT PATTERNS:\n");
    iterator = pattern_itr_begin(r, 1 /* OUTPUT */ );
    while (pattern_itr_at(iterator, &mp))
    {
        fprintf(outfile,"%d %s\n",mp.type,mp.pattern);
        iterator = pattern_itr_next(iterator);
    }
    fclose(outfile);
}

