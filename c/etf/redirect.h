/** \file redirect.h
    \brief Redirection Library service API.
*/

#ifndef __REDIRECT_H
#define __REDIRECT_H

#include "list.h"
#include "linkdef.h"
#include "messageq.h"
#include "message_router.h"
#include "registrar.h"
#include "common.h"
#include "threadspec.h"


/*! \mainpage DDL Framework & Redirection Library API Documentation
    \section intro_sec Introduction
    DDL framework is an event driven framework for developing tools which
    monitor and/or intercept dynamic linking of function calls in an
    application. Fundamentally, in DDL, an event is nothing but a dynamic
    link request generated by application. DDL framework makes it easy for
    tool developers to 'listen' to dynamic link resolution events and
    'redirect' function calls to different symbols using infrastructure
    libraries such as, Redirection Library. Moreover, DDL-Framework provides
    esoteric features such as table-based redirection and TCL script
    wrappers. This allows more than one link requests getting redirected
    to a single tracing wrapper function and it could invoke a TCL script
    to perform its work!
    \section component_sec Components of DDL Framework
    \subsection step1 1: Event Dispather
        \li eventdispatch.h defines Event Dispatcher service API.
    \subsection step2 2: Tools
        \li eventdispatch.h defines master as well as slave tool's mandatory interface
    \subsection step3 3: Redirection Library
        \li redirect.h defines Redirection Library service API
        \li linkdef.h defines LinkDef Library API
        \li hash.h defines HashTable Library API
        \li list.h defines List Library API
        
    \htmlinclude footer.html
*/

//! Redirects a symbol to another symbol.
/*! This function takes a symbol which calling library (shared object)
    is trying to resolve at runtime and redirects it to another symbol
    defined in another shared object.
\param linkdef A pointer to the instance of \e LinkDef structure which
               redirection library is maintaining. \b Nonnull.
\param link_funcname The symbol you want to redirect. \b Nonnull.
\param caller_libname The Library trying to resolve the symbol. \b Nonnull.
\param def_funcname The symbol you want to redirect to. \b Nonnull.
\param def_libname The library in which \e def_funcname is defined. \b Nonnull.
\return A pointer to the \e link in given \e LinkDef data structure
        representing this link-def pair. NULL on failure.
\sa redirect_symtotable()
*/

ListElement *redirect_symtosym(LinkDef *linkdef,
                               const char *link_funcname,
                               const char *caller_libname,
                               const char *def_funcname,
                               const char *def_libname);


//! Redirects a symbol to a table.
/*! This function takes a symbol which calling library (shared object)
    is trying to resolve at runtime and redirects it to a table
    defined in another shared object.
\param linkdef A pointer to the instance of \e LinkDef structure which
               redirection library is maintaining. \b Nonnull.
\param link_funcname The symbol you want to redirect. \b Nonnull.
\param caller_libname The Library trying to resolve the symbol. \b Nonnull.
\param def_funcname The symbol you want to redirect to. \b Nonnull.
\param def_libname The library in which \e def_funcname is defined. \n Nonnull.
\param tabledef If \e DEFINITION pointer to the the table is already known,
       then table definition entry lookup (on def_funcname and def_libname)
       can be skiped to speed up redirection.
\return A pointer to the \e link in given \e LinkDef data structure
        representing this link-tabledef pair. If redirection to table
        fails (if table is full) \e offset is zero in link otherwise
        it is nonzero.
\sa redirect_symtosym()
*/
ListElement *redirect_symtotable(LinkDef *linkdef,
                                 const char *link_funcname,
                                 const char *caller_libname,
                                 const char *def_funcname,
                                 const char *def_libname,
                                 ListElement *tabledef);


//! Allow for ignoring the next linkup request for a certain function name.
/*! This only ignores ONE linkup request for the named function.
    The string is set empty after that linkup request. Typically
    this is used in a wrapper to  call the original function.
    In the wrapper you would precede the calling of the original
    function with redirect_ignore_next("func","libname"), and then
    after the original function returns you would reset it back by doing
    redirect_ignore_next("",""). Reseting back to NULL is very important
    (since you don't really know if a linkup happened or not).
\param fname Name of the function to ignore linkup request of.
\param lname Name of library calling the function.
\return Nothing.
*/
void  redirect_ignore_next(char* fname,char *lname);

void notification_receive(Message *m, char *pattern);
int request_receive (Message *m, char *pattern);
void del_message_func(Message *m);

/**
    \brief Maintains thread specific ignorenext information

    Ignore_next function needs to maintain the state on per therad  basis. 
    It uses the \e threadspec library to maintain this thread specific information.
    Thus, each \e ThreadSpecific_IgnoreNextInfo "is-a" \e ThreadSpecificDataElement.
    Threfore, the threaspec API can be applied safely on the \e ThreadSpecific_IgnoreNextInfo
    structure.
 */
typedef struct ThreadSpecific_IgnoreNextInfo_tag
{
    ThreadSpecificDataElement _tsde;  //!< for the use of threadspec library.
    char *linkfname;                  //!< a function name to ignore redirection.
    char *linklibname;                //!< library in which a call to above function is made.
} ThreadSpecific_IgnoreNextInfo;


void copy_ignorenext_info_func(ThreadSpecific_IgnoreNextInfo *dest,
                               const ThreadSpecific_IgnoreNextInfo *source);

void destroy_ignorenext_info_func(ThreadSpecific_IgnoreNextInfo *e);


#endif


