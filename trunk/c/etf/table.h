/** \file table.h
    \brief Implements Table-based redirection service API (private).
*/

#ifndef __TABLE_H
#define __TABLE_H

typedef struct 
{
    char *fname, *libname;
    int offset;
    int (*func_ptr)();
} Link_table;

/**
    \brief Dummy parameter structure for \e _jt_jumppoint
    This structure should be atleast as big as the largest
    size of parameter block of the functions getting
    redirected to the table.
*/
struct jt_args {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
   int a0;
   int a1;
   int a2;
   int a3;
   int a4;
   int a5;
   int a6;
   int a7;
   int a8;
   int a9;
   int a10;
   int a11;
   int a12;
   int a13;
   int a14;
   int a15;
#endif /* DOXYGEN_SHOULD_SKIP_THIS */ 
};

//! The jump table itself.
/*! This function is just a repetition of movl and jump instructions.
    When any function is redirected to a table, it gets redirected
    to one of the move instructions in this function. A value is assigned
    to \e _jt_func_index and then it jumps to \e _jt_jumppoint() function.
\return Nothing.
\sa _jt_jumppoint()
*/
void _jt_jumptable(void);

//! Makes a new entry for a new function into table.
/*! This function makes a new entry in the table for the given \e funcname.
    The offset returned is to be added in the address of \e _jt_jumptable
    to get exact location of jumptable. For every function passed, a new entry
    is made.
\param funcname Function to be added in the table.
\param caller_libname Points to library name calling the function
\return Offset for that function. -1 if table is full.
\sa _jt_jumppoint(), _jt_jumptable()
*/
int _jt_get_tableoffset(const char *funcname,const char *caller_libname);

//! Tracing function used by _jt_jumptable().
/*! This is the function where every redirected function
    call lands after going through \e _jt_jumptable.
    The _jt_func_index represents the index of actual function
    getting redirected.
\param args Dummy parameter structure.
\return Value returned by actual function called inside
        _jt_jumppoint().
\sa _jt_jumptable()
*/
int _jt_jumppoint ();

//! Deinitialise table.
/*! This is called when table is unloaded from memory. Therefore,
    it frees its entire data structure.
\return Nothing.
*/
void _fini(void);

#endif


