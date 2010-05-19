#ifndef __THREADSPEC_H
#define __THREADSPEC_H

#include <pthread.h>
#include "generic_list.h"

/**
    \brief A structure to make another structure storable in a thread specific way.

    This functionality of this structure is based on the philosophy of doing
    object oriented programming in a non-object oriented programming language.
    If you want to store a few instances of a structure in a thread specific way, 
    all you need to do is to "inherit" the \e ThreadSpecificDataElement structure 
    in your structure. Because C does not provide language level support for inheritance,
    it needs to be programmed. When \e ThreadSpecificDataElement is declared as the first
    data field in a structure, that structure can be used to store thread specific data using
    the API provided by \e threadspec library. It is very important to note that a
    \e ThreadSpecificDataElement data field must be the first data field because the threadspec
    library depends upon the memory layout of the structure. In this way, your custom structure
    has an "is-a" relationship with ThreadSpecificDataElement. Therefore, it is quite similar to
    the concept of inheritance and polymorphism. I would say it is one way of doing object oriented
    programming in C.
 */
typedef struct ThreadSpecificDataElement_tag
{
    Glist_element _g1; //!< For the use of the generic list.
    int tid;           //!< The thread id representing the thread to which this instance of the structure  belongs.
    int calldepth;     //!< Call depth of the thread (unused)
} ThreadSpecificDataElement;

/**
    \brief A data structure to maintain a list of thread specific data.

    An instance of this structure manages a homogeneous list of \e ThreadSpecificDataElements.
    A ThreadSpecificDatElement is the one which has an "is-a" relationship with the structure
    ThreadSpecificDataElements, i.e. it strictly defines one ThreadSpecificDatElement as the first
    data field.  Thus this structure can maintain a list of "unseen" structure polymorphically
    as long it follows the memory layout of ThreadSpecificDataElement striclty.
 */
typedef struct ThreadSpecificStorage_tag
{
    pthread_mutexattr_t mutexattr;   //!< recmutex attributes
    pthread_mutex_t  recmutex;       //!< A mutex to seralize all the accesses to this structure.
    int elementsize;                 //!< Size of the element to be stored in the list. 
                                     //!< Remember it is polymorphic. Therefore, it need to know its size */
    GlistCopyElementFuncType copy_fptr; //!< A pointer to a function to copy the polymorphic data element.
    GlistCompareElementFuncType compare_fptr; //!< A pointer to a function to compare two polymorphic data elements.
    GlistDestroyElementFuncType destroy_fptr;//!< A pointer to a function to destroy a polymorphic data element.
    GLIST(ThreadSpecificDataElement) datalist; //!< A list of polymorphic ThreadSpecificDataElements. 
} ThreadSpecificStorage;

/**
    \brief TSS means ThreadSpecificStorage

    An important function of this macro is to bring extra clarity and readability to the code
    by hiding the polymorphic behavior of the thread specific storage library. For example, 
    if the macro is used with a structure called "Mystruct" (which has an "is-a" relationship with
    the ThreadSpecificDataElement) then, TSS(Mystruct) gets translated to just ThreadSpecificStorage 
    by the preprocessor and "Mystruct" information is lost. For a programmer, it is better to use TSS(Mystruct)
    than ThreadSpecificStorage because it clearly says what kind of structure is being stored in a
    thread specific way. Otherwise, a simple declaration using ThreadSpecificStorage might leave a 
    programmer guessing.
 */
#define TSS(x) ThreadSpecificStorage

//! Initializes a thread specific storage.
/*! Initializes a thread specific storage.
\param tss A pointer to an instance of ThreadSpecificStorage structure.
\param elementsize The size of the elements which will be held polymorphically by the ThreadSpecificStorage.
\param copy_fptr A pointer to a function which can copy one element of the structure stored polymorphically to the other structure.       
\param destroy_fptr A pointer to a function which can destroy one element of the structure stored polymorphically.
\return Nothing.
\sa tss_deinit()
 */
void tss_init(ThreadSpecificStorage *tss, int elementsize,
              GlistCopyElementFuncType copy_fptr,
              GlistDestroyElementFuncType destroy_fptr);
void tss_deinit(ThreadSpecificStorage *tss);
int tss_erase(ThreadSpecificStorage *tss, int thread_id);
int tss_add(ThreadSpecificStorage *tss, ThreadSpecificDataElement *tsde, int thread_id);
int tss_search(ThreadSpecificStorage *tss, ThreadSpecificDataElement *tsde);
int tss_search_tid(ThreadSpecificStorage *tss, int tid);
int tss_duplicate_if_found(ThreadSpecificStorage *tss, 
                           int tid, ThreadSpecificDataElement *dest_tsde);

//! A \e helper function to compare two thread ids.
/*! This is a \e helper function which is not invoked directly. It is invoked by data structure
    libraries such as, generic list. This function compares two thread ids.
\param to_search A pointer to the first ThreadSpecificDataElement.
\param inlinst A pointer to the second ThreadSpecificDataElement.
\return 1 if a match is found, zero otherwise.
 */

int compare_tids(const ThreadSpecificDataElement *to_search,
                 const ThreadSpecificDataElement *inlist);


/*
void copy_element(void *dest,const void *source);
int compare_elements(const void *to_search,const void *inlist);
void destroy_element(void *e);
*/




#endif

