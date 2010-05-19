#include <assert.h>
#define __USE_UNIX98 /* for pthread_mutexattr_settype */
#include <pthread.h>
#include "threadspec.h"

void tss_init(ThreadSpecificStorage *tss, int elementsize,
              GlistCopyElementFuncType copy_fptr,
              GlistDestroyElementFuncType destroy_fptr)
{
    assert(tss && elementsize);
    pthread_mutexattr_init(&tss->mutexattr);
    pthread_mutexattr_settype(&tss->mutexattr,PTHREAD_MUTEX_RECURSIVE_NP);
    pthread_mutex_init(&tss->recmutex,&tss->mutexattr);
    tss->elementsize=elementsize;
    tss->copy_fptr=copy_fptr;
    tss->destroy_fptr=destroy_fptr;
    Glist_initialise(&tss->datalist);
}

void tss_deinit(ThreadSpecificStorage *tss)
{
    assert(tss);
    pthread_mutex_destroy(&tss->recmutex);
    pthread_mutexattr_destroy(&tss->mutexattr);
    tss->elementsize=0;
    Glist_deinitialise(&tss->datalist,tss->destroy_fptr);
}

int tss_duplicate_if_found(ThreadSpecificStorage *tss, 
                           int tid, ThreadSpecificDataElement *dest_tsde)
{
    int retval;
    ThreadSpecificDataElement *source_tsde, temp_tsde;
    pthread_mutex_lock(&(tss->recmutex));    
    temp_tsde.tid=tid;
    
    if((source_tsde=Glist_search(&tss->datalist,&temp_tsde,(void *)compare_tids)))
    {
        (*tss->copy_fptr)(dest_tsde,source_tsde);
        retval=1; // FOUND
    }
    else
    {
        retval=0; // NOT FOUND
    }
    pthread_mutex_unlock(&tss->recmutex);
    return retval;
}

int tss_search_tid(ThreadSpecificStorage *tss, int tid)
{
    ThreadSpecificDataElement tsde;
    tsde.tid=tid;
    return tss_search(tss,&tsde);
}

int tss_search(ThreadSpecificStorage *tss, ThreadSpecificDataElement *tsde)
{
    int retval;
    pthread_mutex_lock(&tss->recmutex);
    
    if(Glist_search(&tss->datalist,tsde,(GlistCompareElementFuncType)compare_tids))
        retval=1; // FOUND
    else
        retval=0; // NOT FOUND
    
    pthread_mutex_unlock(&tss->recmutex);
    return retval;
}

int tss_add(ThreadSpecificStorage *tss, ThreadSpecificDataElement *tsde, int thread_id)
{
    int retval;
    pthread_mutex_lock(&tss->recmutex);
    
    tsde->tid=thread_id;
    if(Glist_search(&tss->datalist,tsde,(void *)compare_tids))
        retval=0; // Already present. Not added.
    else
    {
        if(Glist_add(&tss->datalist,tsde,tss->elementsize,(void *)tss->copy_fptr))
            retval=1; // added successfully. 
    }
    pthread_mutex_unlock(&tss->recmutex);
    return retval;
}

int tss_erase(ThreadSpecificStorage *tss, int thread_id)
{
    int retval;
    ThreadSpecificDataElement temp_tsde, *source_tsde;
    
    pthread_mutex_lock(&tss->recmutex);    
    temp_tsde.tid=thread_id;
    
    if((source_tsde=Glist_search(&tss->datalist,&temp_tsde,(void *)compare_tids)))
    {
        if(Glist_eraseptr(&tss->datalist,source_tsde,
          (GlistDestroyElementFuncType) tss->destroy_fptr))
                retval=0;  // ERASE FAILED      
        else
                retval=1; // FOUND and DELETED
    }
    else
        retval=0; // NOT FOUND
    
    pthread_mutex_unlock(&tss->recmutex);
    return retval;
}

int compare_tids(const ThreadSpecificDataElement *to_search,
                 const ThreadSpecificDataElement *inlist)
{
    return (to_search->tid == inlist->tid);
}


/**********************************************************************/
//            TEST PROGRAM
/**********************************************************************/
/*

typedef struct Mydata
{
    ThreadSpecificDataElement _tsde;
    int i;
    double d;
} Mydata;

void copy_function(Mydata *dest,const Mydata *source);
void destroy_function(Mydata *e);

int main(void)
{
    TSS(Mydata) mydata_tss;
    Mydata m1, m2, m3;
    Mydata mcp;
    m1.i=m1.d=10;
    m2.i=m2.d=20;
    m3.i=m3.d=30;
    tss_init(&mydata_tss,sizeof(Mydata),(void *)copy_function,(void *)destroy_function);
    
    printf("%d\n",tss_add(&mydata_tss,(void *)&m1,100));
    printf("%d\n",tss_add(&mydata_tss,(void *)&m2,200));
    printf("%d\n",tss_add(&mydata_tss,(void *)&m3,300));
    
    if(tss_search_tid(&mydata_tss,400))
        printf("FOUND:\n");
    else
        printf("NOT FOUND\n");
        
    tss_duplicate_if_found(&mydata_tss,200,(void *)&mcp);
    printf("%d:%lf\n",mcp.i,mcp.d);
    
    tss_deinit(&mydata_tss);
    return 0;
}

void copy_function(Mydata *dest,const Mydata *source)
{
    *dest=*source;
}

void destroy_function(Mydata *e)
{
}
*/

