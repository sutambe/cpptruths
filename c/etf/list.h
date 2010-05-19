/** \file list.h
    \brief List service API, typedefs and enumerations.
*/

#ifndef __LIST_H
#define __LIST_H

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif
/**
    \brief Type of an element in the heterogeneous linked list.
*/
typedef enum { LINK=100, SYMBOL=150, DEFINITION=200 } ElementType;

/**
    \brief Memory ownership of all the string pointers in the ListElement.

    When memory ownership changes are requested or when entire data structure
    is destroyed at program end, if type is MEM_OWNER, strings inside the
    structure are freed, otherwise not. User is not supposed to
    manipulate this field.
*/
typedef enum { MEM_OWNER=10, MEM_REFERENCE=20 } StringMemoryType;

/**
    \brief An element in the heterogeneous linked list.

    This structure is the building block of entire linkdef data structure
    of the program.
*/
typedef struct ListElement_tag
{
   ElementType element_type; //!< Type of an element in the heterogeneous linked list.
   StringMemoryType strmem;  //!< Memory ownership of all the string pointers in the ListElement.
   struct ListElement_tag *next; //!< A pointer to the next ListElement.
                                  /*!< List manipulation operations (find, insert) work using
                                       this pointer only. */

   union
   {
      /**  \brief Holds information about a function call in a program.

           Linkdef structure can maintain many-to-one link-def mapping. It means
           many links may point to same definition.
      */
      struct
      {
         const char *funcname; //!< Holds link's function name.
         const char *libname;  //!< Holds library name with absolute path
         unsigned *GOT_addr;   //!< Hold the address of GOT entry for current link.
         unsigned offset;      //!< Offset if any (in case of table based redirection)
         struct ListElement_tag *currentdef; //!< Points to the definition of link.
         struct ListElement_tag *backlink;   /*!< Points to the list of other links
                                                   (if any) which might point to same
                                                   definition.*/
      } link;
      struct
      {
         const unsigned *address; //!< Function's beginning address.
         struct ListElement_tag *backlink; /*!< Points to the list of possible links
                                                 (if any) which might to the definition. */
         const char *table_name; //!< Tablename in case of table based redirection.
         const char *funcname;   //!< Holds definition function name.
         const char *libname;    //!< Holds definition function library name with full path
      } def;
      struct
      {
        const char *libname; //!< Holds symbol's library name with full path
        const char *sym_name; //!< Holds symbol name.
        const unsigned *sym_addr; //!< Address of the entity the symbol is refering to.
        const char *caller_libname; //!< Library of the caller.
      }symbol;
   } element; //!< Union of 3 different types of ListElements.
} ListElement;

/** \brief A list structure with a count of elements. This holds
    a pointer to the head element (if any).
*/
typedef struct List_tag
{
    int count;
    ListElement *head;
} List;

/**
    \brief A pointer to a function which copies \e source element to \e dest element.

    \e CopyElementFuncType allows user to take control of copy operation. User controls
    deepcopy/shallowcopy semantics through this function pointer. A deepcopy may
    be required when copying string pointers. The user has the responsibility
    to provide the right function pointer.

    \sa CompareElementFuncType, DestroyElementFuncType
*/
typedef void (*CopyElementFuncType)(ListElement *dest,const ListElement *source);

/**
    \brief A pointer to a function which compares two list elements.

    \e CompareElementFuncType allows user to take control of list element comparison.
    It signifies a pointer to a user implemented function which compares two
    ListElements and returns 1 if equal and 0 otherwise. The user has the
    responsibility to provide the right function pointer.

    \sa CopyElementFuncType, DestroyElementFuncType
*/
typedef int  (*CompareElementFuncType)(const ListElement *to_search,const ListElement *inlist);

/**
    \brief A pointer to a function which destroy's a \e ListElement.

    \e DestroyElementFuncType allows user to take control of list element destruction.
    When link_def datastructure is destroyed, all dynamically allocated ListElements
    need to be destroyed. DestroyElementFuncType signifies a pointer to a user
    implemented function which is supposed to free any dynamically allocated strings
    in the ListElement. The user has the responsibility to provide the right
    function pointer.

    \sa CompareElementFuncType, CopyElementFuncType
*/
typedef void (*DestroyElementFuncType)(ListElement *);


//! Initialises a list.
/*! This function must be called once before any other list operation
    is performed on the list.
\param l Pointer to the list structure. \b Nonnull.
\return Nothing
\sa list_deinitialise()
*/
void list_initialise(List *l);

//! Erases all the elements in the list.
/*! This function erases all the elements and calls \e destroy to
    free the contents of \e ListElement.
\param l Pointer to the list structure. \b Nonnull.
\param destroy A pointer to a function which is called for every
       element in the list. It is not supposed to free the
       element pointer passed to it. It is freed inside
       \e list_deinitialise. You may erase contents of \e ListElement
       in function pointed by \e destroy. Even \e next pointer.
\return Nothing
\sa list_initialise()
*/
void list_deinitialise(List *l,DestroyElementFuncType destroy);

//! Returns whether list is empty or not.
/*! This function returns one if list has non-zero elements, zero
    otherwise.
\param l Pointer to a list structure. \b Nonnull.
\return An integer, zero or one.
\sa list_initialise(), list_deinitialise()
*/
int list_empty(const List *l);

//! Adds a copy of given list element to the list.
/*! This function allocates memory for a new \e ListElement. Next,
    user given \e copy function is called to copy the contents
    of \e source \e ListElement into this newly created \e destination
    \e ListElement. \e ListElement given as 2nd parameter is
    unchanged as long as  user given \e copy function does not modify it.

    The idea here is that, the list operations (search, add) are
    independent of \e ListElement contents. Some \e ListElements may
    require non-trivial copy semantics. For example, pointers holding
    malloc'ed memory may need a deepcopy.
\param l Pointer to the list you wanna add into. \b Nonnull.
\param element Pointer to \e prototype ListElement. \b Nonnull.
       A copy of this ListElement is added into the list.
\param copy Pointer to a function which accepts 2 \e ListElement pointer
       parameters. \b Nonnull. This function is supposed to
       copy the contents of the \e source \e ListElement into \e destination
       \e ListElement.
\return Nothing
\sa list_initialise(), list_deinitialise(), list_search()
*/
ListElement * list_add(List *l,const ListElement *element,CopyElementFuncType copy);

//! Removes an element from given list, if found.
/*! This function searches a copy of the dummy \e element.
    If found, it removes it from list and deallocates memory
    allocated for it. Before deleting memory of element in the list
    itself, \e destroy is called to free any member string pointers.
\param l Pointer to the list. \b Nonnull.
\param element Pointer to \e prototype ListElement. \b Nonnull.
       A copy of this ListElement is searched and deleted from the list.
\param equal Pointer to a function which accepts two \e ListElement
       pointer parameters. \b Nonnull. This function is supposed
       to compare the two \e ListElements and return 1 or 0 based
       on result of comparison.
\param destroy Pointer to function which 'knows' how to 'destroy' the element.
       Some list elements may require non-trivial destruction semantics.
       For example, pointers holding malloc'ed memory need to be freed.
       User gets control of it through function \e destroy.
\return Zero if deletion is successful, non-zero otherwise.
\sa list_search(), list_add()
*/
int list_erase(List *l,const ListElement *element,CompareElementFuncType equal,DestroyElementFuncType destroy);

//! Searches a copy of given \e ListElement in the given list.
/*! This function finds first (and only) copy of given \e ListElement.
    \b Equality is determined by user given \e equal function.

    Every element in the list is passed to the \e equal function
    one by one, along with the element to search. Function \e equal
    is supposed to compare the keys in the elements and return 1
    if two keys are same, 0 otherwise.

\param l Pointer to the list you want to search. \b Nonnull.
\param to_search Pointer to a dummy \e ListElement which holds
       same \b key as you are looking for. \b Nonnull.
\param equal Pointer to a function which accepts two \e ListElement
       pointer parameters. \b Nonnull. This function is supposed
       to compare the two \e ListElements and return 1 or 0 based
       on result of comparison.
\return Pointer to the \e ListElement, if found. NULL otherwise.
\sa list_initialise(), list_deinitialise(), list_add()
*/
ListElement * list_search(const List *l,const ListElement *to_search,CompareElementFuncType equal);

//! Initialises a ListElement.
/*! Sets \b sizeof(ListElement) bytes pointed by \e element
    to zero. This function is especially useful when you want
    to search an element having particular key.<br>
    1. Create an \e auto variable of type \e ListElement.<br>
    2. Initialise it with list_element_initialise()<br>
    3. Set the key you are looking for in the element field(s).<br>
    4. Call list_search() with \e to_search pointing to the \e auto
       variable in step 1.
\warning 
    <p><b style="color: red;">Presently list_element_initialise()
    assigns MEM_REFERENCE to element->strmem.</b></p>
\param element Pointer to the ListElement. \b Nonnull.
\return Nothing
\sa list_element_deinitialise()
*/
void list_element_initialise(ListElement *element);

//! Deinitialises a ListElement.
/*! It calls user given \e destroy function on \e element.
\param element Pointer to the ListElement you want to destroy. \b Nonnull.
\param destroy Pointer to function which 'knows' how to 'destroy' the element.
       Some list elements may require non-trivial destruction semantics.
       For example, pointers holding malloc'ed memory need to be freed.
       User gets control of it through function \e destroy.
\return Nothing
\sa list_element_initialise()
*/
void list_element_deinitialise(ListElement *element,DestroyElementFuncType destroy);

/**
    \brief Defines the type for the list of links (or defs) iterator.
 */
typedef ListElement * ListIterator;

/**
    \brief Defines the type for the list of links (all of which share the same definition) iterator.
                   
    Important thing to note here is that BackListIterator iterates over all the links which share
    a common definition. This is especially useful in case of table-based redirection in which
    multiple links share the same definition. 
 */
typedef ListElement * BackLinkIterator;

//! Begins an iterator at the first element of the list.
/*! Begins an iterator at the first element of the list.
\param list A pointer to the list to traverse the iterator on.
\return A nonnull list iterator if list is not empty, null otherwise.
\sa list_itr_begin_at(), list_itr_at(), list_itr_next()
 */
ListIterator list_itr_begin(const List *list);

//! Begins an iterator at the specified element in the list if found.
/*! Begins an iterator at the specified element in the list if found.
\param list A pointer to the list to traverse the iterator on.
\param to_search An element to search in the list. If this element is found
       the iterator begins at this element.
\param equal A pointer to function which compares two ListElements.
\return A nonnull list iterator if list is not empty and the element is found, null otherwise.
\sa list_itr_begin(), list_itr_at(), list_itr_next()
 */
ListIterator list_itr_begin_at(const List *list,const ListElement *to_search,
                               CompareElementFuncType equal);

//! Returns a pointer to the \e ListElement pointed to by the iterator
/*! Returns a pointer to the \e ListElement pointed to by the iterator
\param itr The list iterator.
\return If itr is nonnull it returns the ListElement in the list pointed by the iterator.
\sa list_itr_begin(), list_itr_begin_at(), list_itr_next()
 */
ListElement *list_itr_at(ListIterator itr);

//! Advances the iterator to the next element in the list.
/*! Advances the iterator to the next element in the list.
\param itr The list iterator.
\return A nonnull list iterator if there are more elements in the list, null otherwise.
\sa list_itr_begin(), list_itr_begin_at(), list_itr_at()
 */
ListIterator list_itr_next(ListIterator itr);

//! Begins an iterator at the beginning of the list of links which share the same definition.
/*! Begins an iterator at the beginning of the list of links which share the same definition.
\param def A pointer to the common definition.
\return A nonnull back link iterator if there are one or more links which share the given definition. NULL otherwise.
\sa backlink_itr_next(), backlink_itr_at()
 */
BackLinkIterator backlink_itr_begin(const ListElement *def);

//! Returns a pointer the list element pointed to by the back link iterator.
/*! Returns a pointer the list element pointed to by the back link iterator.
\param itr The back link iterator
\return If itr is nonnull it returns the ListElement in the list pointed by the iterator.
\sa backlink_itr_next(), backlink_itr_begin()
 */
ListElement *backlink_itr_at(BackLinkIterator itr);

//! Advances the iterator to the next element in the list of the links which share the same defintion.
/*! Advances the iterator to the next element in the list of the links which share the same defintion.
\param itr The back link iterator
\return If itr is nonnull it returns a back link iterator to the next element in the list. Null otherwise.
\sa backlink_itr_at(), backlink_itr_begin()
 */
BackLinkIterator backlink_itr_next(BackLinkIterator itr);


#endif

