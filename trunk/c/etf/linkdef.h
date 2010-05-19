/** \file linkdef.h
    \brief LinkDef service API, typedefs and enumerations.
*/

#ifndef __LINKDEF_H
#define __LINKDEF_H

#include "hash.h"
#include "list.h"


/**
    \brief A single unit of linkdef data structure.

    This structure is the big picture of entire linkdef data structure
    of the program.
*/
typedef struct LinkDef_tag
{
    HashTable links_hash;
    HashTable defs_hash;
    HashTable symbols_hash;
} LinkDef;

//! Initialises the entire linkdef structure.
/*! This function must be called once before any other linkdef operation
    is performed on data.
\param ld Pointer to the linkdef structure. \b Nonnull.
\param size Size of hash tables in the linkdef structure. Greater than zero.
\return Nothing
\sa linkdef_deinitialise()
*/
void linkdef_initialise(LinkDef *ld, int size);

//! Erases the linkdef data structure.
/*!
\param ld Pointer to the linkdef structure. \b Nonnull.
\return Nothing
\sa linkdef_initialise()
*/
void linkdef_deinitialise(LinkDef *ld);

//! Searches an entry for a link in the linkdef data structure.
/*! This function seaches an entry for the \e link_funcname - \e link_libname
    pair in \e LinkDef::links_hash table.
\param ld Pointer to the linkdef structure. \b Nonnull.
\param link_funcname Pointer to the name of \b callee function
       the link represents.
\param link_libname Pointer to the \b caller library name.
\return Pointer to the link element if found. NULL otherwise.
\sa linkdef_find_def(), linkdef_add_link()
*/
ListElement *linkdef_find_link(LinkDef *ld,
                                const char *link_funcname,
                                const char *link_libname);

//! Searches an entry for a definition in the linkdef data structure.
/*! This function seaches an entry for the \e def_funcname - \e def_libname
    pair in \e LinkDef::defs_hash table.
\param ld Pointer to the linkdef structure. \b Nonnull.
\param def_funcname Pointer to the name of \b definition function.
\param def_libname Pointer to the library name which contains the definition
                   of the function.
\return Pointer to the def element if found. NULL otherwise.
\sa linkdef_find_link(), linkdef_add_def()
*/
ListElement *linkdef_find_def(LinkDef *ld,
                                const char *def_funcname,
                                const char *def_libname);

//! Adds a new link entry in the linkdef data structure.
/*! This function adds a new link in the links_hash table of given
    linkdef structure. If \e requested_strmem is MEM_REFERENCE,
    shallow copies of string pointers are made. And if
    \e requested_strmem is MEM_OWNER, deep copies of string
    pointers are made.
\param ld Pointer to the linkdef structure. \b Nonnull.
\param link_funcname Pointer to the name of \b link function.
\param link_libname Pointer to the library name which is calling the function.
\param requested_strmem Ownership of strings in the newly created link element.
\return Pointer to the link element if found. NULL otherwise.
\sa linkdef_find_link(), linkdef_add_def()
*/
ListElement *linkdef_add_link(LinkDef *ld,
                                const char *link_funcname,
                                const char *link_libname,
                                const StringMemoryType requested_strmem);

//! Adds a new def entry in the linkdef data structure.
/*! This function adds a new definition in the defs_hash table of given
    linkdef structure. If \e requested_strmem is MEM_REFERENCE,
    shallow copies of string pointers are made. And if
    \e requested_strmem is MEM_OWNER, deep copies of string
    pointers are made.
\param ld Pointer to the linkdef structure. \b Nonnull.
\param def_funcname Pointer to the name of \b def function.
\param def_libname Library name which defines the function.
\param requested_strmem Ownership of strings in the newly created link element.
\return Pointer to the def element if found. NULL otherwise.
\sa linkdef_find_def(), linkdef_add_link()
*/
ListElement *linkdef_add_def(LinkDef *ld,
                                const char *def_funcname,
                                const char *def_libname,
                                const StringMemoryType requested_strmem);

//! Adds a new table definition entry in the linkdef data structure.
/*! This function adds a new table definition in the defs_hash table of given
    linkdef structure. If \e requested_strmem is MEM_REFERENCE,
    shallow copies of string pointers are made. And if
    \e requested_strmem is MEM_OWNER, deep copies of string
    pointers are made.
\param ld Pointer to the linkdef structure. \b Nonnull.
\param tablename Pointer to the string ID of the table. \b Nonnull.
\param table_funcname Pointer to the name of table function. \b Nonnull.
\param table_libname Pointer to the library name which contains the table function.
\param requested_strmem Ownership of strings in the newly created link element.
\return Pointer to the table definition if created. NULL otherwise.
\sa linkdef_find_table()
*/
ListElement *linkdef_add_table(LinkDef *ld,
                                const char *tablename,
                                const char *table_funcname,
                                const char *table_libname,
                                const StringMemoryType requested_strmem);

//! Searches a table definition entry in the linkdef data structure.
/*! This function finds a table definition in the defs_hash table of given
    linkdef structure.
\param ld Pointer to the linkdef structure. \b Nonnull.
\param tablename Pointer to the string ID of the table. \b Nonnull.
\param table_funcname Pointer to the name of table function. \b Nonnull.
\param table_libname Pointer to the library name which contains the table function.
\return Pointer to the table definition if found. NULL otherwise.
\sa linkdef_add_table()
*/
ListElement *linkdef_find_table(LinkDef *ld,
                                const char *tablename,
                                const char *table_funcname,
                                const char *table_libname);

/*! \brief Searches parent of given \e link in the list of links which point
    to the definition given by \e def.

    This function returns parent \e ListElement of the \e link it is searching.
    The link's parent can be either DEFINITION type or LINK type.
\param link Pointer to the link you want to search the parent of. \b Nonnull.
\param def Pointer to the definition the \e link is pointing to. \b Nonnull.
\return Pointer to \e ListElement if a parent is found. NULL otherwise.
        It is important to note that the parent can be of DEFINITION type
        or LINK type.
\sa linkdef_drop_linkfromdef()
*/
ListElement *linkdef_search_parentof_linktodef(ListElement *link,ListElement *def);

//! Connection of link to def and def to link is erased.
/*! This function removes the def from  \e link's \e currentdef.
    It also unhooks the \e link from the backlink of given \e def.
\param link Pointer to the link you want to search the parent of. \b Nonnull.
\param def Pointer to the definition the \e link is pointing to. \b Nonnull.
\return Pointer to the same \e link which is passed if it was found in the backlink
        of \e def. NULL otherwise.
\sa linkdef_insert_linktodef()
*/
ListElement *linkdef_drop_linkfromdef(ListElement *link, ListElement *def);

//! Connects a \e link to a \e def.
/*! This function changes given \e link's currentdef to given \e def.
    It also hooks the \e link into the backlink of the \e def.
\param link Pointer to the link you want to search the parent of. \b Nonnull.
\param def Pointer to the definition the \e link is pointing to. \b Nonnull.
\return Pointer to the same \e link which is passed.
\sa linkdef_drop_linkfromdef()
*/
ListElement *linkdef_insert_linktodef(ListElement *link, ListElement *def);

//! Changes the string contents of a \e ListElement.
/*! This function changes \e funcname and \e libname strings to the given
    strings. If \e requested_strmem is MEM_REFERENCE, pointers are shallow
    copied. And if \e requested_strmem is MEM_OWNER, deep copies of string
    pointers are made.
\param anylinkordef Pointer to any type of \e ListElement. It can be
       of LINK type or DEFINITION type. \b Nonnull.
\param funcname The function name you want to change the \e ListElement to. \b Nonnull.
\param libname The library name you want to change the \e ListElement to. \b Nonnull.
\param requested_strmem If \e requested_strmem is MEM_REFERENCE, pointers are shallow
       copied. And if \e requested_strmem is MEM_OWNER, deep copies of string
       pointers are made.
\return Pointer to the same \e ListElement which is passed.
*/
ListElement *linkdef_assign(ListElement *anylinkordef,
                             const char *funcname,
                             const char *libname,
                             const StringMemoryType requested_strmem);

//! Erases a link from LinkDef data structure, if found.
/*! This function is stronger than linkdef_drop_linkfromdef().
    First, it finds a link of given names. If found, it erases
    its entry. Apart from disconneting link from its current definition,
    it deallocates memory allocated to the link.
\param ld Pointer to the linkdef structure. \b Nonnull.
\param link_funcname Pointer to the name of \b link function. \b Nonnull.
\param link_libname Library name which calls the function. \b Nonnull.
\return Zero if deletetion is successful, non-zero otherwise.
\sa linkdef_erase_def(), linkdef_drop_linkfromdef(), linkdef_insert_linktodef()
*/
int linkdef_erase_link(LinkDef *ld,const char *link_funcname,
                       const char *link_libname);

//! Erases a definition from LinkDef data structure, if found.
/*! First, it finds a definition of given names. If found, it erases
    its entry ONLY if there are no links pointing to that definition.
    (This means, bakclink is NULL.) If condition is satisfied, it deallocates
    memory allocated to the definition.
\param ld Pointer to the linkdef structure. \b Nonnull.
\param def_funcname Pointer to the name of \b definition function. \b Nonnull.
\param def_libname Library name which defines the symbol. \b Nonnull.
\return Zero if deletetion is successful, non-zero otherwise.
\sa linkdef_erase_link(), linkdef_drop_linkfromdef(), linkdef_insert_linktodef()
*/
int linkdef_erase_def(LinkDef *ld,const char *def_funcname,
                      const char *def_libname);

//! Makes a shallow copy of source into dest.
/*! This function makes a shallow copy of source \e ListElement into
    destination \e ListElement irrespective of the \e strmem says about
    either of the \e ListElements. The destination \e ListElement is
    markd MEM_REFERENCE. Note that it can accept list elements of type
    LINK as well as DEFINITION. Note that this function is supposed
    to be used with list and hash operations which require a callback function
    pointer named \e copy of type void (*)(ListElement *, ListElement *)
\param dest Destination list element. \b Nonnull.
\param source Source list element. \b Nonnull.
\return Nothing
*/
void shallowcopy(ListElement *dest,const ListElement *source);

//! Makes a deep copy of source into dest.
/*! This function makes a deep copy (using strdup) of source \e ListElement into
    destination \e ListElement irrespective of the \e strmem says about
    either of the \e ListElements. The destination \e ListElement is
    markd MEM_OWNER. Note that it can accept list elements of type
    LINK as well as DEFINITION. Note that this function is supposed
    to be used with list and hash operations which require a callback function
    pointer named \e copy of type void (*)(ListElement *, ListElement *)
\param dest Destination list element. \b Nonnull.
\param source Source list element. \b Nonnull.
\return Nothing
*/
void deepcopy(ListElement *dest,const ListElement *source);

//! Compares two list elements.
/*! This function is supposed to compare \e inlist list element to
    \e to_search list element. The result is used by list and hash
    operations; such as list_search, hash_search. List and hash search
    operations guarantee that when a callback is done, the paramerter
    \e inlist points to the \e ListElement \b in the list. Whereas,
    the paramerter \e to_search points to the copy of \e ListElement we
    are searching. Remember that we pass a kind of dummy \e ListElement
    to search operation of list and hash library. Note that it can accept
    list elements of type LINK as well as DEFINITION. Also note that this
    function is supposed to be used with list and hash operations which
    require an \e equal pointer to a function of type \e CompareElementFuncType

    Also, while searching for \e DEFINITIONS, this function will consider
    \e table_name as comparison criterion. It will return true only if
    table name of \e inlist element matches to that of \e to_search element.
\param to_search The dummy \e ListElement we are supposed to compare the
       \e inlist element with. \b Nonnull.
\param inlist Source list element. It is guaranteed \e Nonnull by list
       operations.
\return It should return 1 if comparison is successful, 0 otherwise.
\sa compare_without_tablename()
*/
int compare_with_tablename(const ListElement *to_search,const ListElement *inlist);

//! Compares two list elements.
/*! This function is supposed to compare \e inlist list element to
    \e to_search list element. The result is used by list and hash
    operations; such as list_search, hash_search. List and hash search
    operations guarantee that when a callback is done, the paramerter
    \e inlist points to the \e ListElement \b in the list. Whereas,
    the paramerter \e to_search points to the copy of \e ListElement we
    are searching. Remember that we pass a kind of dummy \e ListElement
    to search operation of list and hash library. Note that it can accept
    list elements of type LINK as well as DEFINITION. Also note that this
    function is supposed to be used with list and hash operations which
    require an \e equal pointer to a function of type \e CompareElementFuncType

    Also, while searching for \e DEFINITIONS, this function will NOT consider
    \e table_name as comparison criterion. It will return true even if
    table name of \e inlist element does NOT match to that of \e to_search element.
\param to_search The dummy \e ListElement we are supposed to compare the
       \e inlist element with. \b Nonnull.
\param inlist Source list element. It is guaranteed \e Nonnull by list
       operations.
\return It should return 1 if comparison is successful, 0 otherwise.
\sa compare_with_tablename()
*/
int compare_without_tablename(const ListElement *to_search,const ListElement *inlist);


//! It is supposed to destroy the contents of \e ListElement \e l.
/*! This function is called as a callback mechanism when any ListElement is
    freed and its contents need to be freed also. It is upto the programmer
    of the function to free the contents of it. This function will be called
    when a list is destroyed. Note that it can accept list elements of type
    LINK as well as DEFINITION. Note that this function is supposed
    to be used with list and hash operations which require a callback function
    pointer named \e destroy of type void (*)(ListElement *)
\param l Pointer to the \e ListElement, the programmer might want to free
       the contents of. It is guaranteed to be \b nonnull by list operations.
\return Nothing
*/
void destroy(ListElement *l);

//! It returns string representation of given \e ListElement.
/*! This function is called before a ListElement is hashed into a hash bucket.
    Pointer to the list element is passed to it and HashTable library
    expects a unique(as fasr as possible) string representation of the list
    element. The uniqueness of the string determines overall performance of
    the HashTable library.
\param elm Pointer to the \e ListElement to be converted into a string.
\return A pointer to a constant character string.
*/
const char *listelement_tostring(const ListElement *elm);

//! It checks whether two paths are pointing at the files of the same name.
/*! This function checks whether the files pointed to by two paths have the 
    same name or not. 
\param s1 The first file name including path
\param s2 The second file name including path
\return 0 if the file names match, -999 otherwise!
 */
int pathendcmp(const char *s1, const char *s2);

//! It returns a pointer the last occurrence of a '/' in the string or to the beginning of the string.
/*! It returns a pointer the last occurrence of a '/' in the string or to the beginning of the string.
\param str The string to search for the last '/'
\return  A pointer to the beginning of the string or at the last occurence of '/'.
 */
const char *pathsubstr(const char *str);

#endif // __LINKDEF_H
