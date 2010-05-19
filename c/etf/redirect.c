/** \file redirect.c
    \brief Implements Redirection Library.
*/

// Generic Redirection Capabilities
//
//=========================================================
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <tcl.h>
#include <tk.h>
#include <assert.h>
#define __USE_GNU /* This is for PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP */
#include <pthread.h>

#include "libredirect.h"
#include "list.h"
#include "hash.h"
#include "linkdef.h"
#include "table.h"
#include "eventdispatch.h"
#include "messageq.h"
#include "message_router.h"
#include "registrar.h"
#include "common.h"
#include "threadspec.h"

#define END_TABLE 500
#define END_LINK 400
#define MAX 4096
#define HASHTABLE_SIZE 5000

#ifdef USE_TCL
#define LD_WRAP_TCL "wrap.tcl"
Tcl_Interp* TclInterp=0;
#endif

int do_redirect=0;
static char *const MAIN_PROGRAM_STRING="main";
pthread_mutex_t linkdef_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

static Router *rtr;
static LinkDef redirect_linkdef;
static int rd_handle;
static sig_t old_signal_handler;
static const char *redirect_envstr;
static const char *specfile_name;
static int op_pattern_id[MAX_PATTERNS];
static TSS(ThreadSpecific_IgnoreNextInfo) ignorenext_tss = { { 0 } };
//static char **ld_lookup_libname;

extern int match_pattern(MessagePattern *mp1, MessagePattern *mp2);

void _fini(void)
{
    do_redirect=0;

    /* Unset all registered hooks */
    //set_ED_IsActiveHook(rd_handle,0);

    //printf("_fini redirect.c\n");
    old_signal_handler=signal(SIGUSR2,old_signal_handler);
    if(SIG_ERR==old_signal_handler)
        fprintf(stderr,"ERROR while handling signal handler\n");
    
    tss_deinit((void *)&ignorenext_tss);
    pthread_mutex_destroy(&linkdef_mutex);
    linkdef_deinitialise(&redirect_linkdef);
}

//------------------------------------------------------------
// mastertool_init: called just once at application startup
// to initialize redirection code
//------------------------------------------------------------

void *mastertool_init(Registrar *reg, int handle, int thread_id)
{
   char *wraplib_envstr=0;
#ifdef USE_TCL 
    char *wraptcl_envstr=0;
#endif
   int redir_val=0;
   do_redirect=0;
   
   linkdef_initialise(&redirect_linkdef,HASHTABLE_SIZE);
   /*
   old_signal_handler=signal(SIGUSR2,redirect_signal_handler);
   if(SIG_ERR==old_signal_handler)
        fprintf(stderr,"ERROR while handling signal handler\n");
   */   
   tss_init((void *)&ignorenext_tss,sizeof(ignorenext_tss),
             (void *)copy_ignorenext_info_func,
             (void *)destroy_ignorenext_info_func);

#ifdef DEBUG
   printf("MASTERTOOL_INIT\n");
#endif
   /* Note that wraplib_open must be done before specfile_read because
      specfile_read assumes that table library has been loaded. */
   wraplib_envstr=wraplib_open();

   // Get LD_WRAP_TCL to see if we are supporting
   // Tcl wrappers
#ifdef USE_TCL
   wraptcl_envstr = getenv("LD_WRAP_TCL");
   if (!wraptcl_envstr  || 0==wraptcl_envstr[0])
      wraptcl_envstr=LD_WRAP_TCL;
#endif
   // Get LD_REDIRECT to see if we should be
   // doing any redirection at all. If it
   // is not defined or empty, global
   // do_redirect is left 0 and nothing else
   // works
   redirect_envstr = getenv("LD_REDIRECT");
#ifdef DEBUG
   printf("LD_REDIRECT=|%s|\n",redirect_envstr);
#endif
   if(!redirect_envstr)
       redir_val=0;
   else 
   {
      int ret=0;
      redir_val=1;
      if(!memcmp(redirect_envstr,"file:",5))
      {
          specfile_name=redirect_envstr+5;
          specfile_read(&redirect_linkdef,specfile_name);
      }
      rtr=reg->router_ptr;
      ret=register_input_pattern(reg,MASTER_TOOL,"REDIRECT_LOOKUP",NOTIFICATION);
      if(ret == -1) 
          printf("FAILURE handle=%d\n",handle);
      register_input_pattern(reg,MASTER_TOOL,"REDIRECT_DEFINITION",NOTIFICATION);
      register_input_pattern(reg,MASTER_TOOL,"REDIRECT_OFFSET",NOTIFICATION);
      register_input_pattern(reg,MASTER_TOOL,"REQUEST_SYM2SYM",REQUEST);
      register_input_pattern(reg,MASTER_TOOL,"REQUEST_SYM2TABLE",REQUEST);

      op_pattern_id[0]=register_output_pattern(reg,MASTER_TOOL,"REPLY_SYM2SYM",REPLY);
      op_pattern_id[1]=register_output_pattern(reg,MASTER_TOOL,"DO_REDIRECTION",NOTIFICATION);
      op_pattern_id[2]=register_output_pattern(reg,MASTER_TOOL,"DO_OFFSET",NOTIFICATION);
      op_pattern_id[3]=register_output_pattern(reg,MASTER_TOOL,"REPLY_SYM2TABLE",REPLY);
      
      #ifdef USE_TCL
      TclInterp = Tcl_CreateInterp();
      #ifdef USE_TK
      Tk_Init(TclInterp);
      #else
      Tcl_Init(TclInterp);
      #endif
      if(TCL_ERROR == Tcl_EvalFile(TclInterp,wraptcl_envstr))
      {
          fprintf(stderr,"ERROR in Tcl_EvalFile(TclInterp,%s)\n",wraptcl_envstr);
          exit(1);
      }
      #endif
        
      rd_handle=handle;
      set_mastertool_isactive_callback(redirect_isactive);
      #ifdef DEBUG
      printf("Redirection Library initialisation is complete\n");   
      #endif
   }
   do_redirect=redir_val;
   return (void *)&redirect_linkdef;
}

void notification_receive(Message *m, char *pattern)
{
    char *retname=0;
    int ret_offset=0;
    Message message = { { 0 } };
    
    //printf("MASTER_TOOL notification_receive: GOT %s\n",pattern);
    if (strstr(pattern,"REDIRECT_LOOKUP"))
    {
        //printf("redirect.c: REDIRECT_LOOKUP\n");
        retname=redirect_lookup(m->data[0], m->data[1], m->data[2],
                                (int) m->data[3]);
        new_notification(rtr, &message, MASTER_TOOL, SUCCESS, SYNC_MODE,
                         op_pattern_id[1], del_message_func);
        message.data_entries=1;
        message.data[0]=retname;
        send_message(rtr, &message);
    }
    else if (strstr(pattern,"REDIRECT_OFFSET"))
    {
        //printf("redirect.c: REDIRECT_OFFSET\n");
        ret_offset=redirect_offset(m->data[0], m->data[1],(int) m->data[2]);
        new_notification(rtr, &message, MASTER_TOOL, SUCCESS, SYNC_MODE,
                         op_pattern_id[2], del_message_func);
        message.data_entries=1;
        message.data[0]=(void *)ret_offset;
        send_message(rtr, &message);
    }
    else if (strstr(pattern,"REDIRECT_DEFINITION"))
    {
        //printf("redirect.c: REDIRECT_DEFINITION\n");
       
        redirect_definition(m->data[0],m->data[1],m->data[2],
                            m->data[3],m->data[4],m->data[5],(int) m->data[6]);
    }
}

int request_receive (Message *m, char *pattern)
{
    Message message = { { 0 } };
    if (strstr(pattern,"REQUEST_SYM2SYM"))
    {
        redirect_symtosym(&redirect_linkdef,m->data[0],m->data[1],
                          m->data[2], m->data[3]);
        new_reply (rtr, &message, MASTER_TOOL, SUCCESS, SYNC_MODE, 
                   m->reqrepid, op_pattern_id[0], del_message_func);
        send_message(rtr, &message);
     }
     else if (strstr(pattern,"REQUEST_SYM2TABLE"))
     {
        redirect_symtotable(&redirect_linkdef,m->data[0],m->data[1],
                            m->data[2], m->data[3],0);
        new_reply (rtr, &message, MASTER_TOOL, SUCCESS, SYNC_MODE, 
                   m->reqrepid, op_pattern_id[3], del_message_func);
        send_message(rtr, &message);
     }

    return 0; // SUCCESS 
}

void del_message_func(Message *m)
{
    // free contents of m only if they were allocated dynamically.
}

//------------------------------------------------------------
// redirect_isactive() is an external application routine that returns
// non-zero if redirection is desired. If it returns 0, no other redirection
// code is executed. This is dynamic, so it can be turned on and off.
//------------------------------------------------------------
int redirect_isactive(EventType e, int thread_id)
{
#ifdef DEBUG
   printf("redirect_isactive: %d\n",do_redirect);
#endif
   return do_redirect;
}

//------------------------------------------------------------
// Allow for ignoring the next linkup request for a certain
// function name. This only ignores ONE linkup request for
// the named function -- the string is set empty after that
// linkup request. Typically this is used in a wrapper to
// call the original function -- in the wrapper you would
// precede the calling of the original function with
// redirect_ignore_next("func"), and then after the original
// function returns you would reset it back by doing
// redirect_ignore_next("") or redirect_ignore_next(0). Reseting back
// to NULL is very important (since you don't really know if
// a linkup happened or not).
//------------------------------------------------------------

/* This is an old function 
void redirect_ignore_next(char *fname,char *lname)
{
   int temp_do_redirect=do_redirect;
   do_redirect = 0;
   assert(fname && lname);
   strcpy(redirect_ignore_next_funcname,fname);
   strcpy(redirect_ignore_next_libname,lname);
   do_redirect = temp_do_redirect;
}
*/

void redirect_ignore_next(char *fname,char *lname)
{
    int temp_do_redirect=do_redirect;
    ThreadSpecific_IgnoreNextInfo tsini;
      
    do_redirect = 0;
    assert(fname && lname);
   
    if((*fname == 0) || (*lname == 0))
    {
        if(tss_search_tid((void *)&ignorenext_tss,pthread_self()))
        {
            tss_erase((void *)&ignorenext_tss,pthread_self());
        }
    }
    else
    {
        tsini.linkfname=fname;
        tsini.linklibname=lname;
        if(0 == tss_add((void *)&ignorenext_tss,(void *) &tsini,pthread_self()))
        {
            // TID is already present. Therefore, delete old entry and add new 
            // one for the same TID.
            tss_erase((void *)&ignorenext_tss,pthread_self());
        }
        if(0 == tss_add((void *)&ignorenext_tss,(void *)&tsini,pthread_self()))
        {
            // TID is still present!!. This is unexpected. Hunt it!!!
        }
        // Everything is fine.
    }
    do_redirect = temp_do_redirect;
}

//------------------------------------------------------------
// redirect_lookup() is an external application function that does the
// actual renaming. This function takes the name of the function to be
// resolved as a parameter and returns either a) the new name to which
// it should be redirected; b) the exact same pointer as it received as
// a parameter, indicating no redirection is taking place; or c) a NULL
// pointer, also indicating no redirection.
//------------------------------------------------------------
char* redirect_lookup(char* funcname, char *libname,char **lookup_libname, int thread_id)
{
   char* ret_name=0;
   int temp_do_redirect=do_redirect;
   ListElement *link=0, *def=0;
   ThreadSpecific_IgnoreNextInfo tsini;

   do_redirect = 0;
   assert(funcname && libname && lookup_libname);

#ifdef MUTEX
     pthread_mutex_lock(&linkdef_mutex);
#endif

#ifdef DEBUG
   printf("redirect_lookup (%s) from file (%s)\n",funcname,*libname ? libname :  MAIN_PROGRAM_STRING);
#endif
   
if(tss_duplicate_if_found((void *)&ignorenext_tss, thread_id,(void*)&tsini))
   {
        // An entry is FOUND to ignore stored in tsini.
        
       if(*tsini.linkfname &&
           *tsini.linklibname &&
           !strcmp(tsini.linkfname,funcname) &&
           !pathendcmp(tsini.linklibname,libname))
        {   // The entry matches with current link request. It will be ignored now.
            link=linkdef_find_link(&redirect_linkdef,funcname,*libname ? libname : MAIN_PROGRAM_STRING);
            if(!link)
                link=linkdef_add_link(&redirect_linkdef,funcname,*libname ?
                        libname : MAIN_PROGRAM_STRING,MEM_OWNER);
            else
            {
                /* It is important to drop the connection between link and def
                here because redirect_definition is going to reconnect it only
                if there is no previous connection */
                linkdef_drop_linkfromdef(link,link->element.link.currentdef);
            }
            *lookup_libname=0;
            tss_erase((void *)&ignorenext_tss,thread_id);
            destroy_ignorenext_info_func(&tsini);
            do_redirect = temp_do_redirect;
            ret_name=funcname;
        }
   }
   else
   {
        // DEBUG printf("\n\n lookup: before !link\n\n");
        link=linkdef_find_link(&redirect_linkdef,funcname,*libname ? libname : MAIN_PROGRAM_STRING);
        //printf("%d link=%d\n",pathendcmp(libname,"libc.so.6"),(int)link);
        // TODO This part is important for "* l" and "f *" semantics.
        if(!link)
        {
            ListElement *starlink=0;
            starlink=linkdef_find_link(&redirect_linkdef,"*",*libname ? libname : MAIN_PROGRAM_STRING);
            starlink=starlink ?: linkdef_find_link(&redirect_linkdef,funcname,"*");
            starlink=starlink ?: linkdef_find_link(&redirect_linkdef,"*","*");
            if(starlink)
            {
                link=redirect_linktodef(&redirect_linkdef,funcname,*libname ? libname : MAIN_PROGRAM_STRING,
                                        0,0,starlink->element.link.currentdef);
            }
        }
        
        if(!link)
        {
            /* redirect_definition must get invoked by linker to add the corresponding 
                definition entry in the linkdef data structure. Following calls adds only 
                a link entry. Be careful in multi-threaded environment. If one thread
                turns off global redirection, still another thread in the middle of 
                hook invocation must complete invocation of all the hooks.*/
            link=linkdef_add_link(&redirect_linkdef,funcname,
                                    *libname ? libname : MAIN_PROGRAM_STRING,MEM_OWNER);
            *lookup_libname=0;
            ret_name = (char *)funcname;
        }
        else if(link && !link->element.link.currentdef)
        {
            *lookup_libname=0;
            ret_name = (char *)funcname;
        }
        else
        {
            def=link->element.link.currentdef;
            //redirect_show(link);
            //redirect_show(def);
            if(def->element.def.table_name && !link->element.link.offset)
            {
                int tableoffset=0;
                tableoffset=redirect_get_tableoffset(link->element.link.funcname,
                                                    link->element.link.libname,
                                                    def->element.def.libname);
        
                if(-1 == tableoffset)
                {
                    // Table is full. We don't redirect the link but it is important to
                    // save this link like any other saved link because
                    // redirect_definition updates definition in the subsequesnt call.
                    //printf("TABLE OVERFLOW\n");
                    linkdef_drop_linkfromdef(link,link->element.link.currentdef);
                    link->element.link.offset=0;
        
                    *lookup_libname=0;
                    ret_name = (char *)funcname;
                }
                else
                {
                    link->element.link.offset=tableoffset;
                    *lookup_libname=(char *)def->element.def.libname;
                    ret_name = (char *) def->element.def.funcname;
                }
            }
            else
            {
                *lookup_libname=(char *)def->element.def.libname;
                ret_name = (char *) def->element.def.funcname;
            }
        }
   }
#ifdef MUTEX
     pthread_mutex_unlock(&linkdef_mutex);
#endif

   do_redirect = temp_do_redirect;
   return ret_name;
}


//------------------------------------------------------------
// redirect_offset() is an external application function that provides
// the offset (in bytes) that will be added to the address of the symbol
// name that was used. This allows application-level PLT-style
// table-based redirection.
//------------------------------------------------------------
int redirect_offset(const char *funcname, const char *libname, int thread_id)
 {
   int temp_do_redirect=do_redirect;
   int ret = 0;
   ListElement *link=0;

   do_redirect = 0;
   assert(funcname && libname);
#ifdef MUTEX
     pthread_mutex_lock(&linkdef_mutex);
#endif

#ifdef DEBUG
   printf("redirect_offset (%s) from file (%s)\n",funcname,*libname ? libname : MAIN_PROGRAM_STRING);
#endif

   link=linkdef_find_link(&redirect_linkdef,funcname,*libname ? libname : MAIN_PROGRAM_STRING);

   if(link)
   {
       ret = link ? link->element.link.offset : 0;
   }
   else
   {
       fprintf(stderr,"redirect_offset: link not found, TURNING OFF REDIRECTION.\n");
       temp_do_redirect = 0;
       ret = 0;
   }

#ifdef MUTEX
     pthread_mutex_unlock(&linkdef_mutex);
#endif

   do_redirect=temp_do_redirect;
   return ret;
}

int redirect_definition(const char *new_funcname,
                            const char *new_libname,
                            const unsigned *new_func_address,
                            const char *orig_funcname,
                            const char *orig_libname,
                            const unsigned *orig_func_GOT_addr,
                            int thread_id)
{
    ListElement *link=0,*def=0;
    int temp_do_redirect = do_redirect;
    int retval=0;

    do_redirect = 0;
    assert(new_funcname && new_libname && new_func_address);
    assert(orig_funcname && orig_func_GOT_addr);
    
#ifdef MUTEX
     pthread_mutex_lock(&linkdef_mutex);
#endif

#ifdef DEBUG
   printf("redirect_definition (%s):(%s) to (%s):(%s)\n",
          orig_funcname,*orig_libname ? orig_libname : MAIN_PROGRAM_STRING,
          new_funcname,*new_libname ? new_libname : MAIN_PROGRAM_STRING);
#endif

    link=linkdef_find_link(&redirect_linkdef,orig_funcname,
                           *orig_libname ? orig_libname : MAIN_PROGRAM_STRING);

    if(!link)
    {
        retval = FALSE;
    }
    else
    {
        def=link->element.link.currentdef;
        if(!def)
        {
            /* redirect_lookup adds a link only. Therefore redirect_definition must
            get invoked by linker to maintain the data structure. It adds a new
            definition entry and connects the link to the def.
            Normal Flow of control goes through this block of code when
            a new link-def entry is saved. */
            def=linkdef_add_def(&redirect_linkdef,new_funcname,new_libname,MEM_OWNER);
            linkdef_insert_linktodef(link,def);
        }
    
        link->element.link.GOT_addr=(unsigned *)orig_func_GOT_addr;
        def->element.def.address=new_func_address;
        
        do_redirect = temp_do_redirect;
        retval = TRUE;
    }
    
#ifdef MUTEX
     pthread_mutex_unlock(&linkdef_mutex);
#endif

    do_redirect = temp_do_redirect;
    return retval;
}

ListElement *redirect_symtosym(LinkDef *linkdef,
                               const char *link_funcname,
                               const char *caller_libname,
                               const char *def_funcname,
                               const char *def_libname)
{
   ListElement *link=0, *def=0;
   ListElement *le=0;
#ifdef DEBUG
   printf("redirect_symtosym (%s) from file (%s)\n",link_funcname,*caller_libname ? caller_libname : MAIN_PROGRAM_STRING);
#endif
    
#ifdef MUTEX
     pthread_mutex_lock(&linkdef_mutex);
#endif

    link=redirect_linktodef(linkdef,link_funcname,caller_libname,
                            def_funcname,def_libname,0);

    link->element.link.offset=0;
    def=link->element.link.currentdef;
    
    if(def && def->element.def.table_name)
    {
       fprintf(stderr,"Standard Link %s@%s getting redirected to table '%s'.\n",
               link->element.link.funcname,link->element.link.libname,
               def->element.def.table_name);
    }
    
    le = redirect_commit(link,def);
    
#ifdef MUTEX
     pthread_mutex_unlock(&linkdef_mutex);
#endif

    return le;
}

ListElement *redirect_symtotable(LinkDef *linkdef,
                                 const char *link_funcname,
                                 const char *caller_libname,
                                 const char *def_funcname,
                                 const char *def_libname,
                                 ListElement *tabledef)
{
    ListElement *link=0, *le=0;
    int tableoffset=0;

#ifdef DEBUG
   printf("redirect_symtotable (%s) from file (%s)\n",link_funcname,*caller_libname ? caller_libname : MAIN_PROGRAM_STRING);
#endif

#ifdef MUTEX
     pthread_mutex_lock(&linkdef_mutex);
#endif

    link=redirect_linktodef(linkdef,link_funcname,caller_libname,
                            def_funcname,def_libname,tabledef);
    if(!tabledef)
        tabledef=link->element.link.currentdef;
    /*
    If same link is redirected to a table more than once,
    everytime it will make a new entry in the table and earlier
    entries will never be used.
    */

    tableoffset=redirect_get_tableoffset(link_funcname,caller_libname,def_libname);
    if(-1 == tableoffset)
    {
       fprintf(stderr,"TABLE OVERFLOW on %s in %s\n",link_funcname,caller_libname);
       linkdef_drop_linkfromdef(link,link->element.link.currentdef);
       link->element.link.offset=0;
       le = link;
    }
    else
    {
        link->element.link.offset=tableoffset;
        le = redirect_commit(link,tabledef);
    }
    
#ifdef MUTEX
     pthread_mutex_unlock(&linkdef_mutex);
#endif

    return le;
}

// It is an internal function (no mutex locking)
ListElement *redirect_linktodef(LinkDef *linkdef,
                                const char *link_funcname,
                                const char *caller_libname,
                                const char *def_funcname,
                                const char *def_libname,
                                ListElement *tabledef)
{
    ListElement *link=0, *def=0;

    assert(linkdef && link_funcname && caller_libname);
    if(!tabledef) assert(def_funcname && def_libname);

    link=linkdef_find_link(linkdef,link_funcname,caller_libname);
    if(!link)
       link=linkdef_add_link(linkdef,link_funcname,caller_libname,MEM_OWNER);
    def=link->element.link.currentdef;
    if(def)
       linkdef_drop_linkfromdef(link,def);
    if(!tabledef)
    {
       /* If we remove tabledef alltogether, then it will be little
          inefficient but it will become more consistent.
          Nonzero tabledef allows us to skip a linkdef_find_def call.
       */

       def=linkdef_find_def(linkdef,def_funcname,def_libname);
       if(!def)
          def=linkdef_add_def(linkdef,def_funcname,def_libname,MEM_OWNER);
       linkdef_insert_linktodef(link,def);
    }
    else  linkdef_insert_linktodef(link,tabledef);

    return link;
}

/* Make sure that before you call this function you have assigned a proper
   offset value to link. It is an internal function (no mutex locking)*/
ListElement *redirect_commit(ListElement *link, ListElement *def)
{
    char *error=0;
    void *lib_handle=0;

    assert(link && def);

    if(!def->element.def.address)
    {
        if(0==strcmp((char *)def->element.def.libname, MAIN_PROGRAM_STRING))
        {
            fprintf(stderr,"redirect_commit: Redirecting to %s\n",MAIN_PROGRAM_STRING);
        }
        else
        { 
            lib_handle=dlopen(def->element.def.libname,RTLD_LAZY);
            if ((error = dlerror()) != NULL)
            {
                fprintf (stderr, "ERROR redirect_commit: %s\n", error);
                exit(EXIT_FAILURE);
            }
            def->element.def.address=dlsym(lib_handle,(char *)def->element.def.funcname);
            if ((error = dlerror()) != NULL)
            {
                fprintf (stderr, "ERROR redirect_commit: %s\n", error);
                exit(EXIT_FAILURE);
            }
            assert(def->element.def.address);
        }
    }
    if(link->element.link.GOT_addr)
        *link->element.link.GOT_addr=(unsigned)def->element.def.address +
                                      link->element.link.offset;
    
    return link;
}


void specfile_read(LinkDef *linkdef, const char *filename)
{
        char spectype[MAX]={ 0 }, error[MAX] = { 0 };
        FILE *infile=0;
        assert(filename);

        infile=fopen(filename,"r");
        if(!infile)
        {
            sprintf(error,"ERROR (%s)",filename);
            perror(error);
            do_redirect = 0;
            return;
        }

        while(1)
        {
            fscanf(infile,"%s",spectype);
            if('#'==*spectype)
            {
                if(EOF==skipline(infile)) break;
                continue;
            }
            else if(0==strcmp(spectype,"TABLE"))
            {
                if(EOF==skipline(infile)) break;
                if(EOF==specfile_table_read(linkdef,infile)) break;
                else continue;
            }
            else if(0==strcmp(spectype,"LINK"))
            {
                if(EOF==skipline(infile)) break;
                if(EOF==specfile_link_read(linkdef,infile)) break;
                else continue;
            }
            else
            {
                fprintf(stderr,"Parse error at '%s' in %s.\n",spectype,filename);
            }
        }
        fclose(infile);
}

int specfile_link_read(LinkDef *linkdef,FILE *infile)
{
        char orig_funcname[MAX] = { 0 };
        char orig_libname[MAX] = { 0 };
        char new_funcname[MAX] = { 0 };
        char new_libname[MAX] = { 0 };

        ListElement *link=0;

        while(1)
        {
            if(EOF==fscanf(infile,"%s",orig_funcname)) return EOF;
            if('#'==*orig_funcname)
            {
                if(EOF==skipline(infile)) return EOF;
                continue;
            }
            else if(0==strcmp("END_LINK",orig_funcname))
            {
                if(EOF==skipline(infile)) return EOF;
                else return END_LINK;
            }
            if(EOF==fscanf(infile,"%s",orig_libname)) return EOF;
            if(EOF==fscanf(infile,"%s",new_funcname)) return EOF;
            if(EOF==fscanf(infile,"%s",new_libname))  return EOF;

            link=redirect_symtosym(linkdef,orig_funcname,orig_libname,new_funcname,new_libname);
            assert(link);
       }
}

int specfile_table_read(LinkDef *linkdef,FILE *infile)
{
        char tablename[MAX] = { 0 };
        char table_funcname[MAX] = { 0 };
        char table_libname[MAX] = { 0 };
        char orig_funcname[MAX] = { 0 };
        char orig_libname[MAX] = { 0 };

        ListElement *tabledef=0, *link=0;

        fscanf(infile,"%s",tablename);
        fscanf(infile,"%s",table_funcname);
        fscanf(infile,"%s",table_libname);

        tabledef=linkdef_find_table(&redirect_linkdef,tablename,table_funcname,table_libname);
        if(!tabledef)
            tabledef=linkdef_add_table(&redirect_linkdef,tablename,table_funcname,table_libname,MEM_OWNER);

        while(1)
        {
            if(EOF==fscanf(infile,"%s",orig_funcname)) return EOF;
            if('#'==*orig_funcname)
            {
                if(EOF==skipline(infile)) return EOF;
                continue;
            }
            else if(0==strcmp("END_TABLE",orig_funcname))
            {
                if(EOF==skipline(infile)) return EOF;
                else return END_TABLE;
            }
            if(EOF==fscanf(infile,"%s",orig_libname)) return EOF;
            link=redirect_symtotable(linkdef,orig_funcname,orig_libname,table_funcname,table_libname,tabledef);
            assert(link);
        }
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

void redirect_show(const ListElement *element)
{
    assert(element);
    switch(element->element_type)
    {
        case LINK:
        {
            printf("LINK ");
            printf("FUNCNAME: %s\n",element->element.link.funcname);
            printf("LIBNAME: %s\n",element->element.link.libname);
            printf("GOT_addr: %p\n",element->element.link.GOT_addr);
            printf("OFFSET: %d\n",element->element.link.offset);
            break;
        }
        case DEFINITION:
        {
            printf("DEFINITION ");
            if(element->element.def.table_name)
               printf("TABLENAME: %s\n",element->element.def.table_name);
            printf("FUNCNAME: %s\n",element->element.def.funcname);
            printf("LIBNAME: %s\n",element->element.def.libname);
            printf("ADDRESS: %p\n",element->element.def.address);
            break;
        }
        case SYMBOL:
        {
            printf("SYMBOL\n");
            printf("Symbol name: %s\n",element->element.symbol.sym_name);
            printf("Caller libname: %s\n",element->element.symbol.caller_libname);
            printf("Libname: %s\n",element->element.symbol.libname);
            printf("Symbol address: %p\n",element->element.symbol.sym_addr);
            break;
        }
    }
}

void redirect_signal_handler(int sig)
{
    int temp_do_redirect = do_redirect;
    do_redirect = 0;

    printf("Received signal %d.\n",sig);
    if (!redirect_envstr)
    {
        do_redirect = temp_do_redirect;
        return;
    }

    specfile_read(&redirect_linkdef,specfile_name);
    signal(SIGUSR2,redirect_signal_handler);
    do_redirect = temp_do_redirect;
}

char *wraplib_open(void)
{
   // Get LD_WRAP_LIB for the name of the application
   // wrapper library (or whatever functions you are
   // redirecting to (default is LD_WRAP_LIB)

   char *wraplib_envstr_fragment=0, *colon_ptr=0;
   char *wraplib_envstr = getenv("LD_WRAP_LIB");
#ifdef DEBUG
   printf("LD_WRAP_LIB=|%s|\n",wraplib_envstr);
#endif
   if(!wraplib_envstr)
   {
       #ifdef DEBUG
           fprintf(stderr,"wraplib_open: No wrapper libs to load.\n");
       #endif
       return 0;
   }
   wraplib_envstr_fragment=wraplib_envstr;
   while(*wraplib_envstr_fragment)
   {
        /* This while loop extracts wrapper libnames from a colon separated list
           of files. This list hold by LD_WRAP_LIB environmental variable. */
        colon_ptr=strchr(wraplib_envstr_fragment,':');
        if (!colon_ptr)
        {
            if (!dlopen(wraplib_envstr_fragment, RTLD_LAZY | RTLD_GLOBAL ))
            {
                fprintf(stderr,"%s\n",dlerror());
                exit(1);
            }
            break;
        }
        else
        {
            *colon_ptr=0;
            if (!dlopen(wraplib_envstr_fragment, RTLD_LAZY|RTLD_GLOBAL))
            {
                fprintf(stderr,"%s\n",dlerror());
                exit(1);
            }
            *colon_ptr=':';
            while(':'==*++colon_ptr);
            wraplib_envstr_fragment=colon_ptr;
        }
   }
   return wraplib_envstr;
}

// This is an internal function
int redirect_get_tableoffset(const char *funcname, 
                             const char *caller_libname,
                             const char *table_libname)
{
   /* This part could be useful for more than one tables.
   void *lib_handle;
   char *error;
   int (*_jt_get_tableoffset)(const char *fname);

   assert(table_libname && funcname);
   lib_handle=dlopen(table_libname,RTLD_LAZY);
   if ((error = dlerror()) != NULL)
   {
      fprintf (stderr, "%s\n", error);
      exit(EXIT_FAILURE);
   }
   _jt_get_tableoffset=dlsym(lib_handle,"_jt_get_tableoffset");
   assert(_jt_get_tableoffset);
   return (*_jt_get_tableoffset)(funcname,caller_libname);
   */
   return _jt_get_tableoffset(funcname,caller_libname);
}

int redirect_symdef(const char *symbol, const char *libname,
                    unsigned *address, const char* caller_libname, int thread_id)
{
   int temp_do_redirect = do_redirect;
   do_redirect = 0;
#ifdef DEBUG
   printf("redirect_symdef (%s) from file (%s)\n",symbol,*libname ? libname : MAIN_PROGRAM_STRING);
#endif

/*
   ListElement element, *sym=0;

   assert(symbol);
   assert(libname);
   assert(address);
   assert(caller_libname);

   printf("_symdef: %s at %p in %s\n",symbol,address,libname);

   //printf("Entered redirect_symdef");
   if (!((symbol[0] == '_') && (symbol[1]=='Z')))
   {
       do_redirect = temp_do_redirect;
       return 2; // Everything is fine but I want to ignore the symbol.
   }

   list_element_initialise(&element);
   element.element_type=SYMBOL;
   element.element.symbol.sym_name=symbol;
   element.element.symbol.libname=libname;
   element.element.symbol.sym_addr=address;
   element.element.symbol.caller_libname=*caller_libname ? caller_libname :
                                         MAIN_PROGRAM_STRING;

   sym=hash_search(&redirect_linkdef.symbols_hash,&element,compare_with_tablename);
   if (!sym)
      sym=hash_add(&redirect_linkdef.symbols_hash,&element,shallowcopy);

   if (!sym)
   {
       do_redirect = temp_do_redirect;
       return 0; // problem here
   }
*/
   do_redirect = temp_do_redirect;
   return 1; /* Everything is fine and symbol was added successfully. */
}


void copy_ignorenext_info_func(ThreadSpecific_IgnoreNextInfo *dest,
                               const ThreadSpecific_IgnoreNextInfo *source)
{
    *dest=*source;
    //dest->linkfname=strdup((char *)source->linkfname);
    //dest->linklibname=strdup((char *)source->linklibname);
}

void destroy_ignorenext_info_func(ThreadSpecific_IgnoreNextInfo *e)
{
    //free(e->linkfname);
    //free(e->linklibname);
}
